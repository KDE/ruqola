/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messagelistview.h"
#include "ruqola.h"
#include "room.h"
#include "ruqolawidgets_debug.h"
#include "rocketchataccount.h"
#include "delegate/messagelistdelegate.h"
#include "dialogs/reportmessagedialog.h"
#include "textpluginmanager.h"
#include "room/plugins/plugintext.h"
#include "room/plugins/plugintextinterface.h"
#include "threadwidget/threadmessagedialog.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QKeyEvent>
#include <QMenu>
#include <QScrollBar>
#include <QIcon>
#include <QClipboard>
#include <QApplication>

#include <KIO/KUriFilterSearchProviderActions>


MessageListView::MessageListView(Mode mode, QWidget *parent)
    : QListView(parent)
    , mMode(mode)
{
    mDebug = !qEnvironmentVariableIsEmpty("RUQOLA_DEBUGGING");
    mMessageListDelegate = new MessageListDelegate(this);
    mMessageListDelegate->setRocketChatAccount(Ruqola::self()->rocketChatAccount());
    mMessageListDelegate->setShowThreadContext(mMode != Mode::ThreadEditing);
    setItemDelegate(mMessageListDelegate);

    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // nicer in case of huge messages
    setWordWrap(true); // so that the delegate sizeHint is called again when the width changes

    // only the lineedit takes focus
    setFocusPolicy(Qt::NoFocus);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::slotVerticalScrollbarChanged);

    // ensure the scrolling behavior isn't jumpy
    // we always single step by roughly one line
    const auto lineHeight = fontMetrics().height() + 10;
    verticalScrollBar()->setSingleStep(lineHeight);
    // the page step depends on the height of the viewport and needs to be reset when the range changes
    // as Qt would otherwise overwrite it internally. We apparently need a queued connection too to ensure our value is set
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &MessageListView::updateVerticalPageStep, Qt::QueuedConnection);
    updateVerticalPageStep();
}

MessageListView::~MessageListView()
{
}

void MessageListView::slotUpdateLastSeen()
{
    viewport()->update();
}

void MessageListView::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom) {
        connect(mRoom, &Room::lastSeenChanged, this, &MessageListView::slotUpdateLastSeen);
    }
}

void MessageListView::slotVerticalScrollbarChanged(int value)
{
    if (value == 0) {
        Q_EMIT loadHistoryRequested();
        //Perhaps finding a better method.
        verticalScrollBar()->setValue(1); //If we are at 0 we can't continue to load history
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
    Ruqola::self()->rocketChatAccount()->switchingToRoom(roomId);
    MessageModel *model = Ruqola::self()->rocketChatAccount()->messageModelForRoom(roomId);
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

void MessageListView::resizeEvent(QResizeEvent *ev)
{
    QListView::resizeEvent(ev);

    // Fix not being really at bottom when the view gets reduced by the header widget becoming taller
    checkIfAtBottom();
    maybeScrollToBottom(); // this forces a layout in QAIV, which then changes the vbar max value
    updateVerticalPageStep();
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

void MessageListView::checkIfAtBottom()
{
    auto *vbar = verticalScrollBar();
    mAtBottom = vbar->value() == vbar->maximum();
}

void MessageListView::maybeScrollToBottom()
{
    if (mAtBottom) {
        scrollToBottom();
    }
}

void MessageListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }
    const bool isSystemMessage = (index.data(MessageModel::MessageType).value<Message::MessageType>() == Message::System)
                                 || (index.data(MessageModel::MessageType).value<Message::MessageType>() == Message::Information);
    if (isSystemMessage) {
        return;
    }

    const bool canMarkAsUnread = (index.data(MessageModel::UserId).toString() != mCurrentRocketChatAccount->userId());

    QMenu menu(this);
    QAction *copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy"), &menu);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, [=]() {
        copyMessageToClipboard(index);
    });
    //TODO fix me we can't pinned message when we are not owner
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
        setAsFavoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")), isStarred ? i18n("Remove as Favorite") : i18n("Set as Favorite"), &menu);
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

    QAction *selectAllAction = new QAction(i18n("Select All"), &menu);
    connect(selectAllAction, &QAction::triggered, this, [=]() {
        slotSelectAll(index);
    });

    QAction *markMessageAsUnReadAction = new QAction(i18n("Mark Message As Unread"), &menu);
    connect(markMessageAsUnReadAction, &QAction::triggered, this, [=]() {
        slotMarkMessageAsUnread(index);
    });

    QAction *showFullThreadAction = new QAction(i18n("Show Full Thread"), &menu);
    connect(showFullThreadAction, &QAction::triggered, this, [=]() {
        slotShowFullThread(index);
    });

    QAction *editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Edit"), &menu);
    connect(editAction, &QAction::triggered, this, [=]() {
        slotEditMessage(index);
    });

    QAction *quoteAction = new QAction(i18n("Quote"), &menu); //TODO add menu
    connect(quoteAction, &QAction::triggered, this, [=]() {
        slotQuoteMessage(index);
    });


    if (mMode == Mode::Editing) {
        // ## Ideally we'd want to show this when the mouse is over the nickname
        QAction *startPrivateConversation = new QAction(i18n("Start a Private Conversation"), &menu);
        connect(startPrivateConversation, &QAction::triggered, this, [=]() {
            slotStartPrivateConversation(index);
        });
        menu.addAction(startPrivateConversation);

        QAction *startDiscussion = new QAction(i18n("Start a Discussion"), &menu);
        connect(startDiscussion, &QAction::triggered, this, [=]() {
            slotStartDiscussion(index);
        });
        menu.addAction(startDiscussion);
        menu.addSeparator();
        if (mCurrentRocketChatAccount->threadsEnabled()) {
            QAction *replyInThreadAction = new QAction(i18n("Reply in Thread"), &menu);
            connect(replyInThreadAction, &QAction::triggered, this, [=]() {
                slotReplyInThread(index);
            });
            menu.addAction(replyInThreadAction);

            const QString threadMessageId = index.data(MessageModel::ThreadMessageId).toString();
            if (!threadMessageId.isEmpty()) {
                menu.addSeparator();
                menu.addAction(showFullThreadAction);
            }
        }
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
        menu.addAction(selectAllAction);

        menu.addSeparator();
        if (canMarkAsUnread) {
            menu.addAction(markMessageAsUnReadAction);
        }

        if (deleteAction) {
            menu.addSeparator();
            menu.addAction(deleteAction);
        }
        if (mCurrentRocketChatAccount->hasAutotranslateSupport()) {
            createSeparator(menu);
            const bool isTranslated = index.data(MessageModel::ShowTranslatedMessage).toBool();
            QAction *translateAction = new QAction(isTranslated ? i18n("Show Original Message") : i18n("Translate Message"), &menu);
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
        menu.addAction(copyAction);
        menu.addAction(selectAllAction);
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
        if (canMarkAsUnread) {
            menu.addAction(markMessageAsUnReadAction);
            menu.addSeparator();
        }
        menu.addAction(copyAction);
        menu.addAction(selectAllAction);
        menu.addSeparator();
        QAction *goToMessageAction = new QAction(i18n("Go to Message"), &menu); //Add icon
        connect(goToMessageAction, &QAction::triggered, this, [=]() {
            const QString messageId = index.data(MessageModel::MessageId).toString();
            Q_EMIT goToMessageRequested(messageId);
        });
        menu.addAction(goToMessageAction);
    }

    if (mMessageListDelegate->hasSelection()) {
        const QVector<PluginText *> plugins = TextPluginManager::self()->pluginsList();
        const QString selectedText = mMessageListDelegate->selectedText();
        for (PluginText *plugin : plugins) {
            PluginTextInterface *interface = plugin->createInterface(&menu);
            interface->setSelectedText(selectedText);
            interface->addAction(&menu);
        }
    }

    createSeparator(menu);
    QAction *reportMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("messagebox_warning")), i18n("Report Message"), &menu);
    connect(reportMessageAction, &QAction::triggered, this, [=]() {
        slotReportMessage(index);
    });
    menu.addAction(reportMessageAction);

    if (mDebug) {
        createSeparator(menu);
        QAction *debugMessageAction = new QAction(QStringLiteral("Dump Message"), &menu);
        connect(debugMessageAction, &QAction::triggered, this, [=]() {
            slotDebugMessage(index);
        });
        menu.addAction(debugMessageAction);
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

void MessageListView::mousePressEvent(QMouseEvent *event)
{
    mPressedPosition = event->pos();
    handleMouseEvent(event);
}

void MessageListView::mouseMoveEvent(QMouseEvent *event)
{
    // Drag support
    const int distance = (event->pos() - mPressedPosition).manhattanLength();
    if (distance > QApplication::startDragDistance()) {
        mPressedPosition = {};
        const QPersistentModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            QStyleOptionViewItem options = viewOptions();
            options.rect = visualRect(index);
            if (mMessageListDelegate->maybeStartDrag(event, options, index)) {
                return;
            }
        }
    }
    handleMouseEvent(event);
}

void MessageListView::mouseReleaseEvent(QMouseEvent *event)
{
    handleMouseEvent(event);
}

void MessageListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    handleMouseEvent(event);
}

void MessageListView::createSeparator(QMenu &menu)
{
    if (!menu.isEmpty()) {
        menu.addSeparator();
    }
}

void MessageListView::slotSelectAll(const QModelIndex &index)
{
    mMessageListDelegate->selectAll(viewOptions(), index);
}

void MessageListView::handleMouseEvent(QMouseEvent *event)
{
    const QPersistentModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        QStyleOptionViewItem options = viewOptions();
        options.rect = visualRect(index);
        if (mMessageListDelegate->mouseEvent(event, options, index)) {
            update(index);
        }
    }
}

void MessageListView::slotTranslateMessage(const QModelIndex &index, bool checked)
{
    auto *model = const_cast<QAbstractItemModel *>(index.model());
    model->setData(index, checked, MessageModel::ShowTranslatedMessage);
}

void MessageListView::slotDebugMessage(const QModelIndex &index)
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    //Show debug output.
    qDebug() << " message " << *message;
}

void MessageListView::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mCurrentRocketChatAccount = currentRocketChatAccount;
    mMessageListDelegate->setRocketChatAccount(mCurrentRocketChatAccount);
}

void MessageListView::slotQuoteMessage(const QModelIndex &index)
{
    //TODO
}

void MessageListView::slotEditMessage(const QModelIndex &index)
{
    const QString text = index.data(MessageModel::OriginalMessage).toString();
    const QString messageId = index.data(MessageModel::MessageId).toString();
    Q_EMIT editMessageRequested(messageId, text);
}

void MessageListView::slotShowFullThread(const QModelIndex &index)
{
    const QString threadMessageId = index.data(MessageModel::ThreadMessageId).toString();
    const QString threadMessagePreview = index.data(MessageModel::ThreadMessagePreview).toString();
    QPointer<ThreadMessageDialog> dlg = new ThreadMessageDialog(this);
    dlg->setThreadMessageId(threadMessageId);
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setThreadPreview(threadMessagePreview);
    dlg->setRoom(mRoom);
    dlg->exec();
    delete dlg;
}

void MessageListView::slotMarkMessageAsUnread(const QModelIndex &index)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    mCurrentRocketChatAccount->markMessageAsUnReadFrom(messageId);
}

void MessageListView::slotDeleteMessage(const QModelIndex &index)
{
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to delete this message?"), i18n("Delete Message"))) {
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

void MessageListView::slotStartPrivateConversation(const QModelIndex &index)
{
    const QString userName = index.data(MessageModel::Username).toString();
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

MessageListView::Mode MessageListView::mode() const
{
    return mMode;
}

void MessageListView::slotReplyInThread(const QModelIndex &index)
{
    const QString messageId = index.data(MessageModel::MessageId).toString();
    Q_EMIT replyInThreadRequested(messageId);
}

void MessageListView::updateVerticalPageStep()
{
    verticalScrollBar()->setPageStep(viewport()->height() * 3 / 4);
}
