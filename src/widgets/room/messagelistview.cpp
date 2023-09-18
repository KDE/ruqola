/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistview.h"
#include "chat/followmessagejob.h"
#include "chat/unfollowmessagejob.h"
#include "connection.h"
#include "delegate/messagelistdelegate.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/reportmessagedialog.h"
#include "rocketchataccount.h"
#include "room.h"
#include "roomutil.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "selectedmessagebackgroundanimation.h"
#include "threadwidget/threadmessagedialog.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QScrollBar>

#include <config-ruqola.h>

#if HAVE_TEXT_TRANSLATOR
#include "translatetext/translatetextjob.h"
#include "translatetext/translatorenginemanager.h"
#include <texttranslator_version.h>

#include <TextTranslator/TranslatorMenu>
#endif

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

void MessageListView::slotUpdateLastSeen()
{
    viewport()->update();
}

void MessageListView::setRoom(Room *room)
{
    if (mRoom) {
        disconnect(mRoom, &Room::lastSeenChanged, this, &MessageListView::slotUpdateLastSeen);
        mMessageListDelegate->clearSelection();
    }
    mRoom = room;
    if (mRoom) {
        connect(mRoom, &Room::lastSeenChanged, this, &MessageListView::slotUpdateLastSeen);
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

void MessageListView::goToMessage(const QString &messageId)
{
    auto messageModel = qobject_cast<MessageModel *>(model());
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
    auto oldModel = qobject_cast<MessageModel *>(model());
    if (oldModel) {
        oldModel->deactivate();
    }
    setRoom(room);
    const QString roomId = room->roomId();
    mCurrentRocketChatAccount->switchingToRoom(roomId);
    MessageModel *model = mCurrentRocketChatAccount->messageModelForRoom(roomId);
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
    connect(newModel, &QAbstractItemModel::dataChanged, this, [this](const QModelIndex &topLeft, const QModelIndex &, const QVector<int> &roles) {
        if (roles.contains(MessageModel::OriginalMessageOrAttachmentDescription)) {
            const Message *message = topLeft.data(MessageModel::MessagePointer).value<Message *>();
            if (message) {
                QStringList attachmentIdList;
                const auto attachments{message->attachments()};
                attachmentIdList.reserve(attachments.count());
                for (const auto &attachment : attachments) {
                    attachmentIdList.append(attachment.attachmentId());
                }
                mMessageListDelegate->removeMessageCache(message->messageId(), attachmentIdList);
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
    if (!mRoom) {
        return;
    }
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
    info.roomType = mRoom->channelType();
    if (mMessageListDelegate->contextMenu(options, index, info)) {
        return;
    }
    const auto messageType = index.data(MessageModel::MessageType).value<Message::MessageType>();
    const bool isSystemMessage = (messageType == Message::System) || (messageType == Message::Information) || (messageType == Message::VideoConference);
    QMenu menu(this);
    if (isSystemMessage) {
        if (Ruqola::self()->debug()) {
            addDebugMenu(menu, index);
            menu.exec(event->globalPos());
        }
        return;
    }
    mMessageListDelegate->attachmentContextMenu(options, index, info, &menu);
    const bool canMarkAsUnread = (index.data(MessageModel::UserId).toString() != mCurrentRocketChatAccount->userId());

    auto copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy Message"), &menu);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, [=]() {
        copyMessageToClipboard(index);
    });
    QAction *setPinnedMessage = nullptr;
    if (mCurrentRocketChatAccount->allowMessagePinningEnabled() && mRoom->allowToPinMessage()) {
        const bool isPinned = index.data(MessageModel::Pinned).toBool();
        setPinnedMessage = new QAction(QIcon::fromTheme(QStringLiteral("pin")), isPinned ? i18n("Unpin Message") : i18n("Pin Message"), &menu);
        connect(setPinnedMessage, &QAction::triggered, this, [this, isPinned, index]() {
            slotSetPinnedMessage(index, isPinned);
        });
    }
    QAction *setAsFavoriteAction = nullptr;
    if (mCurrentRocketChatAccount->allowMessageStarringEnabled()) {
        const bool isStarred = index.data(MessageModel::Starred).toBool();
        setAsFavoriteAction =
            new QAction(QIcon::fromTheme(QStringLiteral("favorite")), isStarred ? i18n("Remove as Favorite") : i18n("Set as Favorite"), &menu);
        connect(setAsFavoriteAction, &QAction::triggered, this, [this, isStarred, index]() {
            slotSetAsFavorite(index, isStarred);
        });
    }
    QAction *deleteAction = nullptr;
    if (index.data(MessageModel::CanDeleteMessage).toBool()) {
        deleteAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-delete")), i18n("Delete"), &menu);
        connect(deleteAction, &QAction::triggered, this, [=]() {
            slotDeleteMessage(index);
        });
    }

    auto selectAllAction = new QAction(i18n("Select All"), &menu);
    connect(selectAllAction, &QAction::triggered, this, [=]() {
        slotSelectAll(index);
    });

    auto markMessageAsUnReadAction = new QAction(i18n("Mark Message As Unread"), &menu);
    connect(markMessageAsUnReadAction, &QAction::triggered, this, [=]() {
        slotMarkMessageAsUnread(index);
    });

    auto showFullThreadAction = new QAction(i18n("Show Full Thread"), &menu);
    connect(showFullThreadAction, &QAction::triggered, this, [=]() {
        slotShowFullThread(index);
    });

    auto editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Edit"), &menu);
    connect(editAction, &QAction::triggered, this, [=]() {
        slotEditMessage(index);
    });

    auto quoteAction = new QAction(QIcon::fromTheme(QStringLiteral("format-text-blockquote")), i18n("Quote"), &menu);
    connect(quoteAction, &QAction::triggered, this, [=]() {
        slotQuoteMessage(index);
    });

    auto copyLinkToMessageAction = new QAction(i18n("Copy Link To Message"), &menu); // TODO add icon
    connect(copyLinkToMessageAction, &QAction::triggered, this, [=]() {
        slotCopyLinkToMessage(index);
    });

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const QString threadMessageId = index.data(MessageModel::ThreadMessageId).toString();
    const bool messageIsFollowing =
        threadMessageId.isEmpty() ? message->replies().contains(mCurrentRocketChatAccount->userId()) : index.data(MessageModel::ThreadMessageFollowed).toBool();

    const auto followingToMessageAction =
        new QAction(messageIsFollowing ? QIcon::fromTheme(QStringLiteral("notifications-disabled")) : QIcon::fromTheme(QStringLiteral("notifications")),
                    messageIsFollowing ? i18n("Unfollow Message") : i18n("Follow Message"),
                    &menu);
    connect(followingToMessageAction, &QAction::triggered, this, [=]() {
        slotFollowMessage(index, messageIsFollowing);
    });

    auto copyUrlAction = [&]() -> QAction * {
        auto options = listViewOptions();
        options.rect = visualRect(index);
        options.index = index;
        const QString url = mMessageListDelegate->urlAt(options, index, viewport()->mapFromGlobal(event->globalPos()));
        if (url.isEmpty() || url.startsWith(QStringLiteral("ruqola:/")))
            return nullptr;
        auto action = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy URL"), &menu);
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
        if (url.startsWith(QLatin1String("ruqola:/user/"))) {
            url.remove(QStringLiteral("ruqola:/user/"));
            if (!RoomUtil::validUser(url)) {
                return {};
            }
        } else {
            return {};
        }
        auto action = new QAction(QIcon::fromTheme(QStringLiteral("documentinfo")), i18n("User Info"), &menu);
        connect(action, &QAction::triggered, this, [url, this]() {
            slotShowUserInfo(url);
        });
        listActions.append(action);
        if (info.editMode) {
            if (info.roomType != Room::RoomType::Direct) {
                if (mCurrentRocketChatAccount->hasPermission(QStringLiteral("create-d"))) {
                    auto startPrivateConversationAction = new QAction(i18n("Start a Private Conversation"), &menu);
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

    if (mMode == Mode::Editing) {
        auto startDiscussion = new QAction(i18n("Start a Discussion"), &menu);
        connect(startDiscussion, &QAction::triggered, this, [=]() {
            slotStartDiscussion(index);
        });
        menu.addAction(startDiscussion);
        menu.addSeparator();
        if (mCurrentRocketChatAccount->threadsEnabled()) {
            auto replyInThreadAction = new QAction(i18n("Reply in Thread"), &menu);
            connect(replyInThreadAction, &QAction::triggered, this, [=]() {
                slotReplyInThread(index);
            });
            menu.addAction(replyInThreadAction);

            const QString threadMessageId = index.data(MessageModel::ThreadMessageId).toString();
            const int threadMessageCount = index.data(MessageModel::ThreadCount).toInt();
            if (!threadMessageId.isEmpty() || threadMessageCount > 0) {
                menu.addSeparator();
                menu.addAction(showFullThreadAction);
            }
        }
        menu.addSeparator();
        menu.addAction(quoteAction);
        menu.addSeparator();
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
        }
        menu.addSeparator();

        if (index.data(MessageModel::CanEditMessage).toBool()) {
            menu.addAction(editAction);
            menu.addSeparator();
        }
        menu.addAction(copyAction);
        if (copyUrlAction) {
            menu.addAction(copyUrlAction);
        }
        menu.addAction(copyLinkToMessageAction);
        menu.addSeparator();
        menu.addAction(selectAllAction);

        menu.addSeparator();
        if (canMarkAsUnread) {
            menu.addAction(markMessageAsUnReadAction);
        }

        menu.addSeparator();
        menu.addAction(followingToMessageAction);

#if HAVE_TEXT_TRANSLATOR
        createTranslorMenu();
        if (!mTranslatorMenu->isEmpty()) {
            menu.addSeparator();
            mTranslatorMenu->setModelIndex(index);
            menu.addMenu(mTranslatorMenu->menu());
        }
#endif

        if (deleteAction) {
            menu.addSeparator();
            menu.addAction(deleteAction);
        }
        if (mCurrentRocketChatAccount->hasAutotranslateSupport() || !message->localTranslation().isEmpty()) {
            createSeparator(menu);
            const bool isTranslated = message->showTranslatedMessage();
            auto translateAction = new QAction(isTranslated ? i18n("Show Original Message") : i18n("Translate Message"), &menu);
            connect(translateAction, &QAction::triggered, this, [=]() {
                slotTranslateMessage(index, !isTranslated);
            });
            menu.addAction(translateAction);
        }
    } else if (mMode == Mode::ThreadEditing) {
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
        }

        menu.addSeparator();
        menu.addAction(quoteAction);
        menu.addSeparator();
        menu.addAction(copyAction);
        if (copyUrlAction) {
            menu.addAction(copyUrlAction);
        }
        menu.addAction(copyLinkToMessageAction);
        menu.addSeparator();
        menu.addAction(selectAllAction);
        if (canMarkAsUnread) {
            menu.addAction(markMessageAsUnReadAction);
            menu.addSeparator();
        }
        if (index.data(MessageModel::CanEditMessage).toBool()) {
            menu.addSeparator();
            menu.addAction(editAction);
        }

        if (deleteAction) {
            menu.addSeparator();
            menu.addAction(deleteAction);
        }
    } else {
#if 0
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
#endif
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
            menu.addSeparator();
        }
        menu.addAction(copyAction);
        if (copyUrlAction) {
            menu.addAction(copyUrlAction);
        }
        menu.addAction(copyLinkToMessageAction);
        menu.addSeparator();
        menu.addAction(selectAllAction);
#if 0
        createTranslorMenu();
        if (!mTranslatorMenu->isEmpty()) {
            menu.addSeparator();
            mTranslatorMenu->setModelIndex(index);
            menu.addMenu(mTranslatorMenu->menu());
        }
#endif
        menu.addSeparator();
        auto goToMessageAction = new QAction(i18n("Go to Message"), &menu); // Add icon
        connect(goToMessageAction, &QAction::triggered, this, [=]() {
            const QString messageId = message->messageId();
            const QString messageDateTimeUtc = index.data(MessageModel::DateTimeUtc).toString();
            Q_EMIT goToMessageRequested(messageId, messageDateTimeUtc);
        });
        menu.addAction(goToMessageAction);
    }

    if (mMessageListDelegate->hasSelection()) {
        addTextPlugins(&menu, mMessageListDelegate->selectedText());
    }

#if HAVE_TEXT_TO_SPEECH
    createSeparator(menu);
    auto speakAction = menu.addAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")), i18n("Speak Text"));
    connect(speakAction, &QAction::triggered, this, [=]() {
        slotTextToSpeech(index);
    });
#endif

    createSeparator(menu);
    auto reportMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("messagebox_warning")), i18n("Report Message"), &menu);
    connect(reportMessageAction, &QAction::triggered, this, [=]() {
        slotReportMessage(index);
    });
    menu.addAction(reportMessageAction);
    if (!userInfoActions.isEmpty()) {
        menu.addSeparator();
        for (auto action : userInfoActions) {
            menu.addAction(action);
        }
    }
    if (Ruqola::self()->debug()) {
        addDebugMenu(menu, index);
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

void MessageListView::addDebugMenu(QMenu &menu, const QModelIndex &index)
{
    createSeparator(menu);
    if (index.isValid()) {
        auto debugMessageAction = new QAction(QStringLiteral("Dump Message"), &menu); // Don't translate it.
        connect(debugMessageAction, &QAction::triggered, this, [=]() {
            slotDebugMessage(index);
        });
        menu.addAction(debugMessageAction);
        createSeparator(menu);
    }
    auto debugRoomAction = new QAction(QStringLiteral("Dump Room"), &menu); // Don't translate it.
    connect(debugRoomAction, &QAction::triggered, this, [=]() {
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
    model->setData(index, checked, MessageModel::ShowTranslatedMessage);
}

void MessageListView::slotDebugMessage(const QModelIndex &index)
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    // Show debug output.
    qDebug() << " message " << *message << " MessageConvertedText " << index.data(MessageModel::MessageConvertedText).toString();
}

void MessageListView::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mCurrentRocketChatAccount = currentRocketChatAccount;
    mMessageListDelegate->setRocketChatAccount(mCurrentRocketChatAccount);
}

void MessageListView::slotFollowMessage(const QModelIndex &index, bool messageIsFollowing)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    if (messageIsFollowing) {
        auto job = new RocketChatRestApi::UnFollowMessageJob(this);
        job->setMessageId(messageId);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        // connect(job, &RocketChatRestApi::FollowMessageJob::followMessageDone, this, &UsersInRoleWidget::slotAddUsersToRoleDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UnFollowMessageJob job";
        }
    } else {
        auto job = new RocketChatRestApi::FollowMessageJob(this);
        job->setMessageId(messageId);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        // connect(job, &RocketChatRestApi::UnFollowMessageJob::followMessageDone, this, &UsersInRoleWidget::slotAddUsersToRoleDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start FollowMessageJob job";
        }
    }
}

void MessageListView::slotCopyLinkToMessage(const QModelIndex &index)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    const QString permalink = generatePermalink(messageId);
    QClipboard *clip = QApplication::clipboard();
    clip->setText(permalink, QClipboard::Clipboard);
}

QString MessageListView::generatePermalink(const QString &messageId) const
{
    QString permalink = mCurrentRocketChatAccount->serverUrl() + QLatin1Char('/') + RoomUtil::generatePermalink(messageId, mRoom->name(), mRoom->channelType());
    if (!permalink.startsWith(QStringLiteral("https://"))) {
        permalink.prepend(QStringLiteral("https://"));
    }
    return permalink;
}

void MessageListView::slotQuoteMessage(const QModelIndex &index)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    QString text = index.data(MessageModel::OriginalMessage).toString();
    const QString permalink = generatePermalink(messageId);
    // qDebug() << " permalink " << permalink;
    if (text.length() > 80) {
        text = text.left(80) + QStringLiteral("...");
    }
    Q_EMIT quoteMessageRequested(permalink, text);
}

void MessageListView::slotEditMessage(const QModelIndex &index)
{
    const QString text = index.data(MessageModel::OriginalMessageOrAttachmentDescription).toString();
    const QString messageId = index.data(MessageModel::MessageId).toString();
    Q_EMIT editMessageRequested(messageId, text);
}

void MessageListView::slotShowFullThread(const QModelIndex &index)
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const QString threadMessageId = message->threadMessageId();
    QString threadMessagePreview = index.data(MessageModel::ThreadMessagePreview).toString();
    const bool threadIsFollowing =
        threadMessageId.isEmpty() ? message->replies().contains(mCurrentRocketChatAccount->userId()) : index.data(MessageModel::ThreadMessageFollowed).toBool();
    QString messageId = threadMessageId;
    if (threadMessageId.isEmpty()) {
        messageId = message->messageId();
        if (threadMessagePreview.isEmpty()) {
            threadMessagePreview = index.data(MessageModel::MessageConvertedText).toString();
        }
    }
    auto dlg = new ThreadMessageDialog(mCurrentRocketChatAccount, this);
    ThreadMessageWidget::ThreadMessageInfo info;
    info.threadMessageId = messageId;
    info.threadMessagePreview = threadMessagePreview;
    info.threadIsFollowing = threadIsFollowing;
    info.room = mRoom;
    const Message tm = index.data(MessageModel::ThreadMessage).value<Message>();
    info.messageThread = tm;
    dlg->setThreadMessageInfo(info);
    dlg->show();
}

void MessageListView::slotMarkMessageAsUnread(const QModelIndex &index)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    mCurrentRocketChatAccount->markMessageAsUnReadFrom(messageId);
}

void MessageListView::slotDeleteMessage(const QModelIndex &index)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to delete this message?"),
                                           i18nc("@title", "Delete Message"),
                                           KStandardGuiItem::del(),
                                           KStandardGuiItem::cancel())) {
        const QString messageId = index.data(MessageModel::MessageId).toString();
        mCurrentRocketChatAccount->deleteMessage(messageId, mRoom->roomId());
    }
}

void MessageListView::slotTextToSpeech(const QModelIndex &index)
{
    QString message = mMessageListDelegate->selectedText();
    if (message.isEmpty()) {
        message = index.data(MessageModel::OriginalMessage).toString();
    }
    if (!message.isEmpty()) {
        Q_EMIT textToSpeech(message);
    }
}

void MessageListView::slotReportMessage(const QModelIndex &index)
{
    QPointer<ReportMessageDialog> dlg = new ReportMessageDialog(this);
    const QString message = index.data(MessageModel::OriginalMessage).toString();
    dlg->setPreviewMessage(message);
    if (dlg->exec()) {
        const QString messageId = index.data(MessageModel::MessageId).toString();
        mCurrentRocketChatAccount->reportMessage(messageId, dlg->message());
    }
    delete dlg;
}

void MessageListView::slotSetAsFavorite(const QModelIndex &index, bool isStarred)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    mCurrentRocketChatAccount->starMessage(messageId, !isStarred);
}

void MessageListView::slotSetPinnedMessage(const QModelIndex &index, bool isPinned)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    mCurrentRocketChatAccount->pinMessage(messageId, !isPinned);
}

void MessageListView::slotStartPrivateConversation(const QString &userName)
{
    Q_EMIT createPrivateConversation(userName);
}

void MessageListView::slotStartDiscussion(const QModelIndex &index)
{
    const QString message = index.data(MessageModel::OriginalMessage).toString();
    const QString messageId = index.data(MessageModel::MessageId).toString();
    Q_EMIT createNewDiscussion(messageId, message);
}

void MessageListView::copyMessageToClipboard(const QModelIndex &index)
{
    QString message = mMessageListDelegate->selectedText();
    if (message.isEmpty()) {
        if (!index.isValid()) {
            return;
        }

        message = index.data(MessageModel::OriginalMessage).toString();
    }

    QClipboard *clip = QApplication::clipboard();
    clip->setText(message, QClipboard::Clipboard);
    clip->setText(message, QClipboard::Selection);
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
    auto messageModel = qobject_cast<MessageModel *>(model());
    if (messageModel) {
        auto animation = new SelectedMessageBackgroundAnimation(messageModel, this);
        animation->setModelIndex(index);
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
    const QString messageId = index.data(MessageModel::MessageId).toString();
    const QString threadPreview = index.data(MessageModel::OriginalMessage).toString();
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
        const QString originalMessage = modelIndex.data(MessageModel::OriginalMessage).toString();
        if (!originalMessage.isEmpty()) {
            // qDebug() << " originalMessage " << originalMessage;
            // qDebug() << " from " << from << " to " << to;
            TranslateTextJob::TranslateInfo info;
            info.from = from;
            info.to = to;
            info.inputText = originalMessage;
            auto job = new TranslateTextJob(this);
            job->setInfo(info);
            connect(job, &TranslateTextJob::translateDone, this, [this, modelIndex, job](const QString &str) {
                auto messageModel = qobject_cast<MessageModel *>(model());
                // qDebug() << " modelIndex " << modelIndex;
                messageModel->setData(modelIndex, str, MessageModel::LocalTranslation);
                // qDebug() << " str" << str;
                job->deleteLater();
            });
#if TEXTTRANSLATE_VERSION < QT_VERSION_CHECK(1, 4, 40)
            connect(job, &TranslateTextJob::translateFailed, this, [this, job](bool, const QString &errorMessage) {
#else
            connect(job, &TranslateTextJob::translateFailed, this, [this, job](const QString &errorMessage) {
#endif

                KMessageBox::error(this, errorMessage, i18n("Translator Error"));
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

#include "moc_messagelistview.cpp"
