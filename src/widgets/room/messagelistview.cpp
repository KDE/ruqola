/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

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
#include "room/plugins/plugintext.h"
#include "room/plugins/plugintextinterface.h"
#include "roomutil.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "textpluginmanager.h"
#include "threadwidget/threadmessagedialog.h"
#include "translator/translatormenu.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QScrollBar>

#include <KIO/KUriFilterSearchProviderActions>

MessageListView::MessageListView(RocketChatAccount *account, Mode mode, QWidget *parent)
    : MessageListViewBase(parent)
    , mMode(mode)
    , mMessageListDelegate(new MessageListDelegate(account, this))
    , mTranslatorMenu(new TranslatorMenu(this))
    , mCurrentRocketChatAccount(account)
{
    mDebug = !qEnvironmentVariableIsEmpty("RUQOLA_DEBUGGING");
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
    const QVector<PluginText *> plugins = TextPluginManager::self()->pluginsList();
    for (PluginText *plugin : plugins) {
        mPluginTextInterface.append(plugin->createInterface(this));
    }
    connect(mMessageListDelegate, &MessageListDelegate::showUserInfo, this, &MessageListView::slotShowUserInfo);
    connect(mMessageListDelegate, &MessageListDelegate::startPrivateConversation, this, &MessageListView::slotStartPrivateConversation);
    connect(mMessageListDelegate, &MessageListDelegate::updateView, this, &MessageListView::slotUpdateView);
    connect(mTranslatorMenu, &TranslatorMenu::translate, this, &MessageListView::slotTranslate);
}

MessageListView::~MessageListView()
{
    qDeleteAll(mPluginTextInterface);
}

void MessageListView::paintEvent(QPaintEvent *e)
{
#if 0
    if (mRoom) {
        if (mRoom->roomIsEmpty()) {
            QPainter p(viewport());

            QFont font = p.font();
            font.setItalic(true);
            p.setFont(font);

            p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, i18n("Room is Empty."));
        } else {
            QListView::paintEvent(e);
        }
    } else {
        QListView::paintEvent(e);
    }
#else
    QListView::paintEvent(e);
#endif
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
    } else if (key == Qt::Key_Home && ev->modifiers() & Qt::ControlModifier) {
        scrollToTop();
        ev->accept();
    } else if (key == Qt::Key_End && ev->modifiers() & Qt::ControlModifier) {
        scrollToBottom();
        ev->accept();
    }
}

void MessageListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
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
    const bool isSystemMessage = (index.data(MessageModel::MessageType).value<Message::MessageType>() == Message::System)
        || (index.data(MessageModel::MessageType).value<Message::MessageType>() == Message::Information);
    if (isSystemMessage) {
        return;
    }
    const bool canMarkAsUnread = (index.data(MessageModel::UserId).toString() != mCurrentRocketChatAccount->userId());

    QMenu menu(this);
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
#if 0 // Add translator menu
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
        if (mCurrentRocketChatAccount->hasAutotranslateSupport()) {
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
        const QString selectedText = mMessageListDelegate->selectedText();
        for (PluginTextInterface *interface : std::as_const(mPluginTextInterface)) {
            interface->setSelectedText(selectedText);
            interface->addAction(&menu);
        }
    }

    createSeparator(menu);
    auto reportMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("messagebox_warning")), i18n("Report Message"), &menu);
    connect(reportMessageAction, &QAction::triggered, this, [=]() {
        slotReportMessage(index);
    });
    menu.addAction(reportMessageAction);

    if (mDebug) {
        createSeparator(menu);
        auto debugMessageAction = new QAction(QStringLiteral("Dump Message"), &menu); // Don't translate it.
        connect(debugMessageAction, &QAction::triggered, this, [=]() {
            slotDebugMessage(index);
        });
        menu.addAction(debugMessageAction);
        createSeparator(menu);
        auto debugRoomAction = new QAction(QStringLiteral("Dump Room"), &menu); // Don't translate it.
        connect(debugRoomAction, &QAction::triggered, this, [=]() {
            // Dump info about room => don't use qCDebug here.
            qDebug() << " *mRoom " << *mRoom;
        });
        menu.addAction(debugRoomAction);
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
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
    const QString text = index.data(MessageModel::OriginalMessage).toString();
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
    dlg->setThreadMessageId(messageId);
    dlg->setThreadPreview(threadMessagePreview);
    dlg->setFollowingThread(threadIsFollowing);
    dlg->setRoom(mRoom);
    dlg->show();
}

void MessageListView::slotMarkMessageAsUnread(const QModelIndex &index)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    mCurrentRocketChatAccount->markMessageAsUnReadFrom(messageId);
}

void MessageListView::slotDeleteMessage(const QModelIndex &index)
{
    if (KMessageBox::Yes
        == KMessageBox::questionYesNo(this,
                                      i18n("Do you want to delete this message?"),
                                      i18nc("@title", "Delete Message"),
                                      KStandardGuiItem::del(),
                                      KStandardGuiItem::cancel())) {
        const QString messageId = index.data(MessageModel::MessageId).toString();
        mCurrentRocketChatAccount->deleteMessage(messageId, mRoom->roomId());
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
}
