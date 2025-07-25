/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistview.h"

#include "actionbuttons/actionbuttonsmanager.h"
#include "actionbuttons/actionbuttonutil.h"
#include "administratordialog/moderationconsole/moderationmessageinfodialog.h"
#include "autogenerateui/autogenerateinteractionutil.h"
#include "chat/deletemessagejob.h"
#include "chat/followmessagejob.h"
#include "chat/pinmessagejob.h"
#include "chat/postmessagejob.h"
#include "chat/reportmessagejob.h"
#include "chat/starmessagejob.h"
#include "chat/unfollowmessagejob.h"

#include "forwardmessage/forwardmessagedialog.h"
#include "misc/appsuiinteractionjob.h"
#include "moderation/moderationdismissreportsjob.h"

#include "connection.h"
#include "delegate/messagelistdelegate.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/reportmessagedialog.h"
#include "moderation/moderationreportsjob.h"
#include "rocketchataccount.h"
#include "room.h"
#include "roomutil.h"
#include "ruqola.h"
#include "ruqola_translatemessage_debug.h"
#include "ruqolawidgets_debug.h"
#include "selectedmessagebackgroundanimation.h"
#include "subscriptions/markroomasunreadjob.h"
#include "threadwidget/threadmessagedialog.h"

#include "actionbuttons/actionbutton.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QScrollBar>

#include "config-ruqola.h"

#if HAVE_TEXT_TRANSLATOR
#include "translatetext/translatetextjob.h"
#include "translatetext/translatorenginemanager.h"
#include <texttranslator_version.h>

#include <TextTranslator/TranslatorMenu>
#endif
using namespace Qt::Literals::StringLiterals;

MessageListView::MessageListView(RocketChatAccount *account, Mode mode, QWidget *parent)
    : MessageListViewBase(parent)
    , mMode(mode)
    , mMessageListDelegate(new MessageListDelegate(account, this))
    , mCurrentRocketChatAccount(account)
{
    if (mCurrentRocketChatAccount) {
        mMessageListDelegate->setRocketChatAccount(mCurrentRocketChatAccount);
    }
    mMessageListDelegate->setShowThreadContext(mMode != Mode::ThreadEditing);
    mMessageListDelegate->setEnableEmojiMenu(mMode != Mode::Moderation);
    setItemDelegate(mMessageListDelegate);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::slotVerticalScrollbarChanged);

    // ensure the scrolling behavior isn't jumpy
    // we always single step by roughly one line
    const auto lineHeight = fontMetrics().height() + 10;
    verticalScrollBar()->setSingleStep(lineHeight);
    // the page step depends on the height of the viewport and needs to be reset when the range changes
    // as Qt would otherwise overwrite it internally. We apparently need a queued connection too to ensure our value is set
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &MessageListView::updateVerticalPageStep, Qt::QueuedConnection);
    updateVerticalPageStep();
    connect(mMessageListDelegate, &MessageListDelegate::showUserInfo, this, &MessageListView::slotShowUserInfo);
    connect(mMessageListDelegate, &MessageListDelegate::startPrivateConversation, this, &MessageListView::slotStartPrivateConversation);
    connect(mMessageListDelegate, &MessageListDelegate::updateView, this, &MessageListView::slotUpdateView);
    connect(mMessageListDelegate, &MessageListDelegate::replyToThread, this, &MessageListView::replyInThreadRequested);
    connect(this, &MessageListView::needToClearSizeHintCache, mMessageListDelegate, &MessageListDelegate::clearSizeHintCache);
}

MessageListView::~MessageListView() = default;

void MessageListView::wheelEvent(QWheelEvent *e)
{
    const int y = e->angleDelta().y();
    if (y > 0) {
        if (verticalScrollBar()->value() <= 1) {
            Q_EMIT loadHistoryRequested();
        }
    }
    MessageListViewBase::wheelEvent(e);
}

void MessageListView::paintEvent(QPaintEvent *e)
{
    if (mRoom && (mRoom->numberMessages() == 0)) {
        QPainter p(viewport());

        QFont font = p.font();
        font.setItalic(true);
        p.setFont(font);
        if (mRoom->channelType() == Room::RoomType::Direct) {
            p.drawText(QRect(0, 0, width(), height()), Qt::AlignHCenter | Qt::AlignTop, i18n("You have joined a new direct message"));
        } else {
            p.drawText(QRect(0, 0, width(), height()), Qt::AlignHCenter | Qt::AlignTop, i18n("Start of conversation"));
        }
    } else {
        QListView::paintEvent(e);
    }
}

void MessageListView::slotUpdateView()
{
    viewport()->update();
}

void MessageListView::setRoom(Room *room)
{
    if (mRoom) {
        disconnect(mRoom, &Room::lastSeenChanged, this, &MessageListView::slotUpdateView);
        mMessageListDelegate->clearSelection();
    }
    mRoom = room;
    if (mRoom) {
        connect(mRoom, &Room::lastSeenChanged, this, &MessageListView::slotUpdateView);
    }
}

void MessageListView::slotVerticalScrollbarChanged(int value)
{
    if (value == 0) {
        Q_EMIT loadHistoryRequested();
        // Perhaps finding a better method.
        verticalScrollBar()->setValue(1); // If we are at 0 we can't continue to load history
    }
}

void MessageListView::goToMessage(const QByteArray &messageId)
{
    auto messageModel = qobject_cast<MessagesModel *>(model());
    Q_ASSERT(messageModel);
    const QModelIndex index = messageModel->indexForMessage(messageId);
    if (index.isValid()) {
        scrollTo(index);
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "Message not found:" << messageId;
    }
}

void MessageListView::setChannelSelected(Room *room)
{
    auto oldModel = qobject_cast<MessagesModel *>(model());
    if (oldModel) {
        oldModel->deactivate();
    }
    setRoom(room);
    const QByteArray roomId = room->roomId();
    mCurrentRocketChatAccount->switchingToRoom(roomId);
    MessagesModel *model = mCurrentRocketChatAccount->messageModelForRoom(roomId);
    setModel(model);
    model->activate();
}

void MessageListView::setModel(QAbstractItemModel *newModel)
{
    QAbstractItemModel *oldModel = model();
    if (oldModel) {
        disconnect(oldModel, nullptr, this, nullptr);
    }
    QListView::setModel(newModel);
    connect(newModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &MessageListView::checkIfAtBottom);
    connect(newModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &MessageListView::checkIfAtBottom);
    connect(newModel, &QAbstractItemModel::modelAboutToBeReset, this, &MessageListView::checkIfAtBottom);
    // Connect to rangeChanged rather than rowsInserted/rowsRemoved/modelReset.
    // This way it also catches the case of an item changing height (e.g. after async image loading)
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &MessageListView::maybeScrollToBottom);

    connect(newModel, &QAbstractItemModel::rowsInserted, this, &MessageListView::modelChanged);
    connect(newModel, &QAbstractItemModel::rowsRemoved, this, &MessageListView::modelChanged);
    connect(newModel, &QAbstractItemModel::modelReset, this, &MessageListView::modelChanged);
    // Clear document cache when message is updated otherwise image description is not up to date
    connect(newModel, &QAbstractItemModel::dataChanged, this, [this](const QModelIndex &topLeft, const QModelIndex &, const QList<int> &roles) {
        if (roles.contains(MessagesModel::OriginalMessageOrAttachmentDescription) || roles.contains(MessagesModel::LocalTranslation)
            || roles.contains(MessagesModel::ShowTranslatedMessage)) {
            const Message *message = topLeft.data(MessagesModel::MessagePointer).value<Message *>();
            if (message) {
                mMessageListDelegate->removeMessageCache(message);
            }
        } else if (roles.contains(MessagesModel::DisplayUrlPreview) || roles.contains(MessagesModel::DisplayAttachment)) {
            const Message *message = topLeft.data(MessagesModel::MessagePointer).value<Message *>();
            if (message) {
                mMessageListDelegate->removeSizeHintCache(message->messageId());
            }
        }
    });

    scrollToBottom();
}

void MessageListView::handleKeyPressEvent(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_PageDown || key == Qt::Key_PageUp) {
        // QListView/QAIV PageUp/PageDown moves the current item, first inside visible bounds
        // before it triggers scrolling around. Let's just let the scrollarea handle it,
        // since we don't show the current item.
        QAbstractScrollArea::keyPressEvent(ev);
        ev->accept();
    } else if (ev->modifiers() & Qt::ControlModifier) {
        if (key == Qt::Key_Home) {
            scrollToTop();
            ev->accept();
        } else if (key == Qt::Key_End) {
            scrollToBottom();
            ev->accept();
        }
    }
}

void MessageListView::createTranslorMenu()
{
#if HAVE_TEXT_TRANSLATOR
    if (!mTranslatorMenu) {
        mTranslatorMenu = new TextTranslator::TranslatorMenu(this);
        connect(mTranslatorMenu, &TextTranslator::TranslatorMenu::translate, this, &MessageListView::slotTranslate);
        connect(Ruqola::self(), &Ruqola::translatorMenuChanged, this, [this]() {
            TranslatorEngineManager::self()->translatorConfigChanged();
            mTranslatorMenu->updateMenu();
        });
    }
#endif
}

void MessageListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        if (Ruqola::self()->debug()) {
            QMenu menu(this);
            addDebugMenu(menu, index);
            menu.exec(event->globalPos());
        }
        return;
    }

    auto options = listViewOptions();
    options.rect = visualRect(index);
    options.index = index;
    MessageListDelegate::MenuInfo info;
    info.editMode = (mMode == Mode::Editing);
    info.globalPos = event->globalPos();
    info.pos = viewport()->mapFromGlobal(event->globalPos());
    info.roomType = mRoom ? mRoom->channelType() : Room::RoomType::Unknown;
    if (mMessageListDelegate->contextMenu(options, index, info)) {
        return;
    }
    const auto messageType = index.data(MessagesModel::MessageType).value<Message::MessageType>();
    const bool isSystemMessage = (messageType == Message::System) || (messageType == Message::Information);
    const bool isVideoConferenceMessage = messageType == Message::VideoConference;
    QMenu menu(this);
    if (isSystemMessage) {
        if (Ruqola::self()->debug()) {
            addDebugMenu(menu, index);
            menu.exec(event->globalPos());
        }
        return;
    }
    mMessageListDelegate->attachmentContextMenu(options, index, info, &menu);
    const bool isNotOwnerOfMessage = (index.data(MessagesModel::UserId).toByteArray() != mCurrentRocketChatAccount->userId());

    auto copyAction = new QAction(&menu);
    copyAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    if (hasSelection()) {
        copyAction->setText(i18nc("@action", "Copy Selection"));
    } else {
        copyAction->setText(i18nc("@action", "Copy Message"));
    }
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, [this, index]() {
        copyMessageToClipboard(index);
    });
    QAction *setPinnedMessage = nullptr;
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->allowMessagePinningEnabled() && mRoom && mRoom->allowToPinMessage()) {
        const bool isPinned = index.data(MessagesModel::Pinned).toBool();
        setPinnedMessage =
            new QAction(QIcon::fromTheme(QStringLiteral("pin")), isPinned ? i18nc("@action", "Unpin Message") : i18nc("@action", "Pin Message"), &menu);
        connect(setPinnedMessage, &QAction::triggered, this, [this, isPinned, index]() {
            slotSetPinnedMessage(index, isPinned);
        });
    }
    QAction *setAsFavoriteAction = nullptr;
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->allowMessageStarringEnabled()) {
        const bool isStarred = index.data(MessagesModel::Starred).toBool();
        setAsFavoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")),
                                          isStarred ? i18nc("@action", "Remove as Favorite") : i18nc("@action", "Set as Favorite"),
                                          &menu);
        connect(setAsFavoriteAction, &QAction::triggered, this, [this, isStarred, index]() {
            slotSetAsFavorite(index, isStarred);
        });
    }
    QAction *deleteAction = nullptr;
    if (index.data(MessagesModel::CanDeleteMessage).toBool()) {
        deleteAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-delete")), i18nc("@action", "Delete"), &menu);
        connect(deleteAction, &QAction::triggered, this, [this, index]() {
            slotDeleteMessage(index);
        });
    }

    auto selectAllAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-select-all")), i18nc("@action", "Select All"), &menu);
    connect(selectAllAction, &QAction::triggered, this, [this, index]() {
        slotSelectAll(index);
    });

    auto markMessageAsUnReadAction = new QAction(QIcon::fromTheme(QStringLiteral("checkmark-symbolic")), i18nc("@action", "Mark Message As Unread"), &menu);
    connect(markMessageAsUnReadAction, &QAction::triggered, this, [this, index]() {
        slotMarkMessageAsUnread(index);
    });

    auto showFullThreadAction = new QAction(i18nc("@action", "Show Full Thread"), &menu);
    connect(showFullThreadAction, &QAction::triggered, this, [this, index]() {
        slotShowFullThread(index);
    });

    auto editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18nc("@action", "Edit"), &menu);
    connect(editAction, &QAction::triggered, this, [this, index]() {
        slotEditMessage(index);
    });

    auto quoteAction = new QAction(QIcon::fromTheme(QStringLiteral("format-text-blockquote")), i18nc("@action", "Quote"), &menu);
    connect(quoteAction, &QAction::triggered, this, [this, index]() {
        slotQuoteMessage(index);
    });

    auto copyLinkToMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18nc("@action", "Copy Link To Message"), &menu);
    connect(copyLinkToMessageAction, &QAction::triggered, this, [this, index]() {
        slotCopyLinkToMessage(index);
    });

    auto forwardMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("mail-forward-symbolic")), i18nc("@action", "Forward Message"), &menu);
    connect(forwardMessageAction, &QAction::triggered, this, [this, index]() {
        slotForwardMessage(index);
    });

    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();

    const QString threadMessageId = index.data(MessagesModel::ThreadMessageId).toString();
    const bool messageIsFollowing = threadMessageId.isEmpty()
        ? (message->replies() && message->replies()->replies().contains(mCurrentRocketChatAccount->userId()))
        : index.data(MessagesModel::ThreadMessageFollowed).toBool();

    const auto followingToMessageAction =
        new QAction(messageIsFollowing ? QIcon::fromTheme(QStringLiteral("notifications-disabled")) : QIcon::fromTheme(QStringLiteral("notifications")),
                    messageIsFollowing ? i18nc("@action", "Unfollow Message") : i18nc("@action", "Follow Message"),
                    &menu);
    connect(followingToMessageAction, &QAction::triggered, this, [this, index, messageIsFollowing]() {
        slotFollowMessage(index, messageIsFollowing);
    });

    auto copyUrlAction = [&]() -> QAction * {
        auto options = listViewOptions();
        options.rect = visualRect(index);
        options.index = index;
        const QString url = mMessageListDelegate->urlAt(options, index, viewport()->mapFromGlobal(event->globalPos()));
        if (url.isEmpty() || url.startsWith(QStringLiteral("ruqola:/")))
            return nullptr;
        auto action = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18nc("@action", "Copy URL"), &menu);
        connect(action, &QAction::triggered, this, [url]() {
            QGuiApplication::clipboard()->setText(url);
        });
        return action;
    }();

    auto userInfoActions = [&]() -> QList<QAction *> {
        QList<QAction *> listActions;
        auto options = listViewOptions();
        options.rect = visualRect(index);
        options.index = index;
        QString url = mMessageListDelegate->urlAt(options, index, viewport()->mapFromGlobal(event->globalPos()));
        if (url.isEmpty())
            return {};
        if (url.startsWith("ruqola:/user/"_L1)) {
            url.remove(QStringLiteral("ruqola:/user/"));
            if (!Utils::validUser(url)) {
                return {};
            }
        } else {
            return {};
        }
        auto action = new QAction(QIcon::fromTheme(QStringLiteral("documentinfo")), i18nc("@action", "User Info"), &menu);
        connect(action, &QAction::triggered, this, [url, this]() {
            slotShowUserInfo(url);
        });
        listActions.append(action);
        if (info.editMode) {
            if (info.roomType != Room::RoomType::Direct) {
                if (mCurrentRocketChatAccount->hasPermission(QStringLiteral("create-d"))) {
                    auto startPrivateConversationAction =
                        new QAction(QIcon::fromTheme(QStringLiteral("document-send-symbolic")), i18nc("@action", "Start a Private Conversation"), &menu);
                    connect(startPrivateConversationAction, &QAction::triggered, this, [this, url]() {
                        slotStartPrivateConversation(url);
                    });
                    auto separator = new QAction(&menu);
                    separator->setSeparator(true);
                    listActions.append(separator);
                    listActions.append(startPrivateConversationAction);
                }
            }
        }
        return listActions;
    }();

    auto threadInfoActions = [&]() -> QList<QAction *> {
        QList<QAction *> listActions;
        if (mCurrentRocketChatAccount->ruqolaServerConfig()->threadsEnabled()) {
            const QString threadMessageId = index.data(MessagesModel::ThreadMessageId).toString();
            const int threadMessageCount = index.data(MessagesModel::ThreadCount).toInt();
            if (!threadMessageId.isEmpty() || threadMessageCount > 0) {
                auto separator = new QAction(&menu);
                separator->setSeparator(true);
                listActions.append(separator);
                listActions.append(showFullThreadAction);
            }
            if (!listActions.isEmpty()) {
                auto separator = new QAction(&menu);
                separator->setSeparator(true);
                listActions.append(separator);
            }
        }

        return listActions;
    }();

    switch (mMode) {
    case Mode::Editing: {
        auto startDiscussion = new QAction(i18nc("@action", "Start a Discussion"), &menu);
        connect(startDiscussion, &QAction::triggered, this, [this, index]() {
            slotStartDiscussion(index);
        });
        menu.addAction(startDiscussion);
        menu.addSeparator();
        auto replyInThreadAction = new QAction(QIcon::fromTheme(QStringLiteral("mail-replied-symbolic")), i18nc("@action", "Reply in Thread"), &menu);
        connect(replyInThreadAction, &QAction::triggered, this, [this, index]() {
            slotReplyInThread(index);
        });
        menu.addAction(replyInThreadAction);
        for (auto action : threadInfoActions) {
            menu.addAction(action);
        }

        if (!isVideoConferenceMessage) {
            menu.addAction(quoteAction);
        }
        menu.addSeparator();
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
        }
        menu.addSeparator();

        if (!isVideoConferenceMessage && index.data(MessagesModel::CanEditMessage).toBool()) {
            menu.addAction(editAction);
            menu.addSeparator();
        }
        menu.addAction(copyAction);
        if (copyUrlAction) {
            menu.addAction(copyUrlAction);
        }
        menu.addAction(copyLinkToMessageAction);
        if (!isVideoConferenceMessage) {
            menu.addAction(forwardMessageAction);
            menu.addSeparator();
            menu.addAction(selectAllAction);
        }

        menu.addSeparator();
        if (isNotOwnerOfMessage) {
            menu.addAction(markMessageAsUnReadAction);
        }

        menu.addSeparator();
        menu.addAction(followingToMessageAction);

#if HAVE_TEXT_TRANSLATOR
        if (!isVideoConferenceMessage) {
            createTranslorMenu();
            if (!mTranslatorMenu->isEmpty()) {
                menu.addSeparator();
                mTranslatorMenu->setModelIndex(index);
                menu.addMenu(mTranslatorMenu->menu());
            }
        }
#endif

        if (deleteAction) {
            menu.addSeparator();
            menu.addAction(deleteAction);
        }
        if (!isVideoConferenceMessage
            && ((mCurrentRocketChatAccount->hasAutotranslateSupport() && mRoom && mRoom->autoTranslate() && !mRoom->autoTranslateLanguage().isEmpty())
                || !message->localTranslation().isEmpty())) {
            createSeparator(menu);
            const bool isTranslated = message->showTranslatedMessage();
            auto translateAction = new QAction(isTranslated ? i18nc("@action", "Show Original Message") : i18nc("@action", "Translate Message"), &menu);
            connect(translateAction, &QAction::triggered, this, [this, index, isTranslated]() {
                slotTranslateMessage(index, !isTranslated);
            });
            menu.addAction(translateAction);
        }
        break;
    }
    case Mode::ThreadEditing: {
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
        }

        if (!isVideoConferenceMessage) {
            menu.addSeparator();
            menu.addAction(quoteAction);
        }
        menu.addSeparator();
        menu.addAction(copyAction);
        if (copyUrlAction) {
            menu.addAction(copyUrlAction);
        }
        menu.addAction(copyLinkToMessageAction);
        if (!isVideoConferenceMessage) {
            menu.addAction(forwardMessageAction);
            menu.addSeparator();
            menu.addAction(selectAllAction);
        }
        if (isNotOwnerOfMessage) {
            menu.addAction(markMessageAsUnReadAction);
            menu.addSeparator();
        }
        if (!isVideoConferenceMessage && index.data(MessagesModel::CanEditMessage).toBool()) {
            menu.addSeparator();
            menu.addAction(editAction);
        }
#if HAVE_TEXT_TRANSLATOR
        if (!isVideoConferenceMessage) {
            createTranslorMenu();
            if (!mTranslatorMenu->isEmpty()) {
                menu.addSeparator();
                mTranslatorMenu->setModelIndex(index);
                menu.addMenu(mTranslatorMenu->menu());
            }
        }
#endif
        if (deleteAction) {
            menu.addSeparator();
            menu.addAction(deleteAction);
        }
        if (!isVideoConferenceMessage
            && ((mCurrentRocketChatAccount->hasAutotranslateSupport() && mRoom && mRoom->autoTranslate() && !mRoom->autoTranslateLanguage().isEmpty())
                || !message->localTranslation().isEmpty())) {
            createSeparator(menu);
            const bool isTranslated = message->showTranslatedMessage();
            auto translateAction = new QAction(isTranslated ? i18nc("@action", "Show Original Message") : i18nc("@action", "Translate Message"), &menu);
            connect(translateAction, &QAction::triggered, this, [this, index, isTranslated]() {
                slotTranslateMessage(index, !isTranslated);
            });
            menu.addAction(translateAction);
        }
        break;
    }
    case Mode::Moderation: {
        auto showReportInfo = new QAction(i18nc("@action", "View Reports"), &menu); // Add icon
        connect(showReportInfo, &QAction::triggered, this, [this, message]() {
            const auto messageId = message->messageId();
            const auto job = new RocketChatRestApi::ModerationReportsJob(this);
            job->setMessageId(messageId);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::ModerationReportsJob::moderationReportsDone, this, [this](const QJsonObject &obj) {
                ModerationReportInfos infos;
                infos.parseModerationReportInfos(obj);
                slotShowReportInfo(std::move(infos));
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportInfoJob job";
            }
        });
        menu.addAction(showReportInfo);
        menu.addSeparator();
        if (copyUrlAction) {
            menu.addAction(copyUrlAction);
        }
        menu.addSeparator();
        menu.addAction(selectAllAction);
        menu.addSeparator();
        auto dismissReports = new QAction(i18nc("@action", "Dismiss Reports"), &menu); // Add icon
        connect(dismissReports, &QAction::triggered, this, [this, message]() {
            const auto messageId = message->messageId();
            const auto job = new RocketChatRestApi::ModerationDismissReportsJob(this);
            job->setMessageId(messageId);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::ModerationDismissReportsJob::moderationDismissReportsDone, this, []() {
                // TODO
                qDebug() << " RocketChatRestApi::ModerationDismissReportsJob::moderationDismissReportsDone ";
                // TODO update element!
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportInfoJob job";
            }
        });

        break;
    }
    case Mode::Viewing: {
#if 0
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
#endif
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
            menu.addSeparator();
        }

        if (!threadInfoActions.isEmpty()) {
            menu.addSeparator();
            for (auto action : threadInfoActions) {
                menu.addAction(action);
            }
        }

        menu.addAction(copyAction);
        if (copyUrlAction) {
            menu.addAction(copyUrlAction);
        }
        menu.addAction(copyLinkToMessageAction);
        if (!isVideoConferenceMessage) {
            menu.addAction(forwardMessageAction);
            menu.addSeparator();
            menu.addAction(selectAllAction);
        }
#if 0
        createTranslorMenu();
        if (!mTranslatorMenu->isEmpty()) {
            menu.addSeparator();
            mTranslatorMenu->setModelIndex(index);
            menu.addMenu(mTranslatorMenu->menu());
        }
#endif
        menu.addSeparator();
        auto goToMessageAction = new QAction(i18nc("@action", "Go to Message"), &menu); // Add icon
        connect(goToMessageAction, &QAction::triggered, this, [this, index, message]() {
            const QByteArray messageId = message->messageId();
            const QString messageDateTimeUtc = index.data(MessagesModel::DateTimeUtc).toString();
            Q_EMIT goToMessageRequested(messageId, messageDateTimeUtc);
        });
        menu.addAction(goToMessageAction);
        break;
    }
    }

    if (mMessageListDelegate->hasSelection()) {
        addTextPlugins(&menu, mMessageListDelegate->selectedText());
    }

#if HAVE_TEXT_TO_SPEECH
    if (!isVideoConferenceMessage) {
        createSeparator(menu);
        auto speakAction = menu.addAction(QIcon::fromTheme(QStringLiteral("text-speak-symbolic")), i18nc("@action", "Speak Text"));
        connect(speakAction, &QAction::triggered, this, [this, index]() {
            slotTextToSpeech(index);
        });
    }
#endif

    if (mMode != Mode::Moderation && isNotOwnerOfMessage) {
        createSeparator(menu);
        auto reportMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("messagebox_warning")), i18nc("@action", "Report Message"), &menu);
        connect(reportMessageAction, &QAction::triggered, this, [this, index]() {
            slotReportMessage(index);
        });
        menu.addAction(reportMessageAction);
    }
    if (!userInfoActions.isEmpty()) {
        menu.addSeparator();
        for (auto action : userInfoActions) {
            menu.addAction(action);
        }
    }
    if (Ruqola::self()->debug()) {
        addDebugMenu(menu, index);
    }
    if (mCurrentRocketChatAccount && mRoom) {
        ActionButton::FilterActionInfo filterInfo;
        filterInfo.buttonContext = ActionButton::ButtonContext::MessageAction;
        filterInfo.roomTypeFilter = ActionButtonUtil::convertRoomTypeToActionButtonRoomTypeFilter(mRoom);
        const QList<ActionButton> actionButtons = mCurrentRocketChatAccount->actionButtonsManager()->actionButtonsFromFilterActionInfo(filterInfo);
        if (!actionButtons.isEmpty()) {
            const QString lang = QLocale().name();
            auto actSeparator = new QAction(this);
            actSeparator->setSeparator(true);
            menu.addAction(actSeparator);
            for (const auto &actionButton : actionButtons) {
                auto act = new QAction(this);
                const QString translateIdentifier = ActionButtonUtil::generateTranslateIdentifier(actionButton);
                const QString appId = QString::fromLatin1(actionButton.appId());
                const QByteArray roomId = mRoom->roomId();
                act->setText(mCurrentRocketChatAccount->getTranslatedIdentifier(lang, translateIdentifier));
                connect(act, &QAction::triggered, this, [this, actionButton, appId, roomId, message]() {
                    auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
                    RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
                    info.methodName = appId;
                    AutoGenerateInteractionUtil::ActionMessageInfo actionMessageInfo;
                    actionMessageInfo.actionId = actionButton.actionId();
                    actionMessageInfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
                    actionMessageInfo.roomId = roomId;
                    actionMessageInfo.messageId = message->messageId();
                    info.messageObj = AutoGenerateInteractionUtil::createMessageActionButton(actionMessageInfo);
                    job->setAppsUiInteractionJobInfo(info);

                    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
                    connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [this](const QJsonObject &replyObject) {
                        Q_EMIT uiInteractionRequested(replyObject);
                    });
                    if (!job->start()) {
                        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppsUiInteractionJob job";
                    }
                });
                menu.addAction(act);
            }
        }
    }

    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

void MessageListView::addDebugMenu(QMenu &menu, const QModelIndex &index)
{
    if (!mRoom) {
        return;
    }
    createSeparator(menu);
    if (index.isValid()) {
        auto debugMessageAction = new QAction(QStringLiteral("Dump Message"), &menu); // Don't translate it.
        connect(debugMessageAction, &QAction::triggered, this, [this, index]() {
            slotDebugMessage(index);
        });
        menu.addAction(debugMessageAction);
        createSeparator(menu);
    }
    auto debugRoomAction = new QAction(QStringLiteral("Dump Room"), &menu); // Don't translate it.
    connect(debugRoomAction, &QAction::triggered, this, [this]() {
        // Dump info about room => don't use qCDebug here.
        qDebug() << " mRoom " << *mRoom;
    });
    menu.addAction(debugRoomAction);
}

bool MessageListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mMessageListDelegate->maybeStartDrag(event, option, index);
}

bool MessageListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mMessageListDelegate->mouseEvent(event, option, index);
}

void MessageListView::createSeparator(QMenu &menu)
{
    if (!menu.isEmpty()) {
        menu.addSeparator();
    }
}

void MessageListView::slotSelectAll(const QModelIndex &index)
{
    mMessageListDelegate->selectAll(listViewOptions(), index);
}

void MessageListView::slotTranslateMessage(const QModelIndex &index, bool checked)
{
    auto model = const_cast<QAbstractItemModel *>(index.model());
    model->setData(index, checked, MessagesModel::ShowTranslatedMessage);
}

void MessageListView::slotDebugMessage(const QModelIndex &index)
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    // Show debug output.
    qDebug() << " message " << *message << " MessageConvertedText " << index.data(MessagesModel::MessageConvertedText).toString();
}

void MessageListView::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mCurrentRocketChatAccount = currentRocketChatAccount;
    mMessageListDelegate->setRocketChatAccount(mCurrentRocketChatAccount);
}

void MessageListView::slotFollowMessage(const QModelIndex &index, bool messageIsFollowing)
{
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
    if (messageIsFollowing) {
        auto job = new RocketChatRestApi::UnFollowMessageJob(this);
        job->setMessageId(messageId);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UnFollowMessageJob job";
        }
    } else {
        auto job = new RocketChatRestApi::FollowMessageJob(this);
        job->setMessageId(messageId);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start FollowMessageJob job";
        }
    }
}

void MessageListView::slotCopyLinkToMessage(const QModelIndex &index)
{
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
    const QString permalink = generatePermalink(QString::fromLatin1(messageId));
    QClipboard *clip = QApplication::clipboard();
    clip->setText(permalink, QClipboard::Clipboard);
}

void MessageListView::slotForwardMessage(const QModelIndex &index)
{
    QPointer<ForwardMessageDialog> dlg = new ForwardMessageDialog(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const QList<QByteArray> identifiers = dlg->channelIdentifiers();
        const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
        auto job = new RocketChatRestApi::PostMessageJob(this);
        job->setText(QStringLiteral("[ ](%1)\n").arg(generatePermalink(QString::fromLatin1(messageId))));
        job->setRoomIds(identifiers);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start PostMessageJob job";
        }
    }
    delete dlg;
}

QString MessageListView::generatePermalink(const QString &messageId) const
{
    if (!mRoom) {
        return {};
    }
    QString permalink = mCurrentRocketChatAccount->serverUrl() + QLatin1Char('/') + RoomUtil::generatePermalink(messageId, mRoom->name(), mRoom->channelType());
    if (!permalink.startsWith(QStringLiteral("https://"))) {
        permalink.prepend(QStringLiteral("https://"));
    }
    return permalink;
}

void MessageListView::slotQuoteMessage(const QModelIndex &index)
{
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
    QString text = index.data(MessagesModel::OriginalMessage).toString();
    const QString permalink = generatePermalink(QString::fromLatin1(messageId));
    // qDebug() << " permalink " << permalink;
    if (text.length() > 80) {
        text = text.left(80) + QStringLiteral("...");
    }
    Q_EMIT quoteMessageRequested(permalink, text);
}

void MessageListView::slotEditMessage(const QModelIndex &index)
{
    const QString text = index.data(MessagesModel::OriginalMessageOrAttachmentDescription).toString();
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
    Q_EMIT editMessageRequested(messageId, text);
}

void MessageListView::slotShowFullThread(const QModelIndex &index)
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    const QByteArray threadMessageId = message->threadMessageId();
    QString threadMessagePreview = index.data(MessagesModel::ThreadMessagePreview).toString();
    const bool threadIsFollowing = threadMessageId.isEmpty()
        ? (message->replies() && message->replies()->replies().contains(mCurrentRocketChatAccount->userId()))
        : index.data(MessagesModel::ThreadMessageFollowed).toBool();
    QByteArray messageId = threadMessageId;
    if (threadMessageId.isEmpty()) {
        messageId = message->messageId();
        if (threadMessagePreview.isEmpty()) {
            threadMessagePreview = index.data(MessagesModel::MessageConvertedText).toString();
        }
    }
    auto dlg = new ThreadMessageDialog(mCurrentRocketChatAccount, Ruqola::self()->parentWidget());
    ThreadMessageWidget::ThreadMessageInfo info;
    info.threadMessageId = messageId;
    info.threadMessagePreview = threadMessagePreview;
    info.threadIsFollowing = threadIsFollowing;
    info.room = mRoom;
    const Message tm = index.data(MessagesModel::ThreadMessage).value<Message>();
    info.messageThread = tm;
    dlg->setThreadMessageInfo(info);
    dlg->show();
}

void MessageListView::slotMarkMessageAsUnread(const QModelIndex &index)
{
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();

    auto job = new RocketChatRestApi::MarkRoomAsUnReadJob(this);
    job->setObjectId(messageId);
    job->setUnReadObject(RocketChatRestApi::MarkRoomAsUnReadJob::MarkAsUnReadObject::FromMessage);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start markMessageAsUnReadFrom job";
    }
}

void MessageListView::slotDeleteMessage(const QModelIndex &index)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to delete this message?"),
                                           i18nc("@title", "Delete Message"),
                                           KStandardGuiItem::del(),
                                           KStandardGuiItem::cancel())) {
        const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();

        auto job = new RocketChatRestApi::DeleteMessageJob(this);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setRoomId(mRoom->roomId());
        job->setMessageId(messageId);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start deleteMessage job";
        }
    }
}

void MessageListView::slotTextToSpeech(const QModelIndex &index)
{
    QString message = mMessageListDelegate->selectedText();
    if (message.isEmpty()) {
        message = index.data(MessagesModel::OriginalMessage).toString();
    }
    if (!message.isEmpty()) {
        Q_EMIT textToSpeech(message);
    }
}

void MessageListView::slotReportMessage(const QModelIndex &index)
{
    QPointer<ReportMessageDialog> dlg = new ReportMessageDialog(this);
    const QString message = index.data(MessagesModel::OriginalMessage).toString();
    dlg->setPreviewMessage(message);
    if (dlg->exec()) {
        const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
        auto job = new RocketChatRestApi::ReportMessageJob(this);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setMessageId(messageId);
        job->setReportMessage(dlg->message());
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start reportMessage job";
        }
    }
    delete dlg;
}

void MessageListView::slotSetAsFavorite(const QModelIndex &index, bool isStarred)
{
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();

    auto job = new RocketChatRestApi::StarMessageJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setStarMessage(!isStarred);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start starMessage job";
    }
}

void MessageListView::slotSetPinnedMessage(const QModelIndex &index, bool isPinned)
{
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();

    auto job = new RocketChatRestApi::PinMessageJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setPinMessage(!isPinned);
    if (!job->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start PinMessageJob";
    }
}

void MessageListView::slotStartPrivateConversation(const QString &userName)
{
    Q_EMIT createPrivateConversation(userName);
}

void MessageListView::slotStartDiscussion(const QModelIndex &index)
{
    const QString message = index.data(MessagesModel::OriginalMessage).toString();
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
    Q_EMIT createNewDiscussion(messageId, message);
}

bool MessageListView::hasSelection() const
{
    return mMessageListDelegate->hasSelection();
}

QString MessageListView::selectedText(const QModelIndex &index)
{
    QString message = mMessageListDelegate->selectedText();
    if (message.isEmpty()) {
        if (!index.isValid()) {
            return {};
        }
        message = index.data(MessagesModel::OriginalMessage).toString();
    }
    return message;
}

void MessageListView::clearTextDocumentCache()
{
    mMessageListDelegate->clearTextDocumentCache();
}

void MessageListView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    disconnect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::slotVerticalScrollbarChanged);
    QListView::scrollTo(index, hint);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::slotVerticalScrollbarChanged);
    addSelectedMessageBackgroundAnimation(index);
}

void MessageListView::addSelectedMessageBackgroundAnimation(const QModelIndex &index)
{
    auto messageModel = qobject_cast<MessagesModel *>(model());
    if (messageModel) {
        auto animation = new SelectedMessageBackgroundAnimation(messageModel, this);
        animation->setModelIndex(index);
        connect(animation, &SelectedMessageBackgroundAnimation::backgroundColorChanged, this, [this, animation]() {
            mMessageListDelegate->needUpdateIndexBackground(animation->modelIndex(), animation->backgroundColor());
            update(animation->modelIndex());
        });
        connect(animation, &SelectedMessageBackgroundAnimation::animationFinished, this, [this, animation]() {
            mMessageListDelegate->removeNeedUpdateIndexBackground(animation->modelIndex());
            update(animation->modelIndex());
        });
        animation->start();

    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << " message model empty";
    }
}

void MessageListView::setSearchText(const QString &str)
{
    mMessageListDelegate->setSearchText(str);
}

MessageListView::Mode MessageListView::mode() const
{
    return mMode;
}

void MessageListView::slotReplyInThread(const QModelIndex &index)
{
    const QByteArray messageId = index.data(MessagesModel::MessageId).toByteArray();
    const QString threadPreview = index.data(MessagesModel::OriginalMessage).toString();
    Q_EMIT replyInThreadRequested(messageId, threadPreview);
}

void MessageListView::slotShowUserInfo(const QString &userName)
{
    DirectChannelInfoDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setUserName(userName);
    dlg.setRoles(mCurrentRocketChatAccount->roleInfo());
    dlg.exec();
}

void MessageListView::slotTranslate(const QString &from, const QString &to, const QPersistentModelIndex &modelIndex)
{
#if HAVE_TEXT_TRANSLATOR
    if (modelIndex.isValid()) {
        const QString originalMessage = modelIndex.data(MessagesModel::OriginalMessage).toString();
        if (!originalMessage.isEmpty()) {
            qCDebug(RUQOLA_TRANSLATEMESSAGE_LOG) << " originalMessage " << originalMessage;
            qCDebug(RUQOLA_TRANSLATEMESSAGE_LOG) << " from " << from << " to " << to;
            TranslateTextJob::TranslateInfo info;
            info.from = from;
            info.to = to;
            info.inputText = originalMessage;
            auto job = new TranslateTextJob(this);
            job->setInfo(info);
            connect(job, &TranslateTextJob::translateDone, this, [this, modelIndex, job](const QString &str) {
                auto messageModel = qobject_cast<MessagesModel *>(model());
                qCDebug(RUQOLA_TRANSLATEMESSAGE_LOG) << " modelIndex " << modelIndex;
                // qCDebug(RUQOLA_TRANSLATEMESSAGE_LOG) << " messageModel " << messageModel;
                messageModel->setData(modelIndex, str, MessagesModel::LocalTranslation);
                qCDebug(RUQOLA_TRANSLATEMESSAGE_LOG) << " translated string :" << str;
                job->deleteLater();
            });
            connect(job, &TranslateTextJob::translateFailed, this, [this, job](const QString &errorMessage) {
                KMessageBox::error(this, errorMessage, i18nc("@title:window", "Translator Error"));
                job->deleteLater();
            });
            job->translate();
        }
    }
#else
    Q_UNUSED(from)
    Q_UNUSED(to)
    Q_UNUSED(modelIndex)
#endif
}

void MessageListView::slotShowReportInfo(const ModerationReportInfos &info)
{
    ModerationMessageInfoDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setReportInfos(info);
    dlg.exec();
}
#include "moc_messagelistview.cpp"
