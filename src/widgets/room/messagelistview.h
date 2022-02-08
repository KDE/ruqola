/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QListView>
#include <QPointer>
#include <QVector>
class MessageListDelegate;
class RocketChatAccount;
class Room;
class PluginTextInterface;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListView : public QListView
{
    Q_OBJECT
public:
    enum class Mode {
        ThreadEditing,
        Editing,
        Viewing, /*when we show list of message as mentions, search etc.*/
    };

    explicit MessageListView(MessageListView::Mode mode, QWidget *parent = nullptr);
    ~MessageListView() override;

    void setRoom(Room *room);

    void setChannelSelected(Room *room);

    void setModel(QAbstractItemModel *newModel) override;

    void handleKeyPressEvent(QKeyEvent *ev);

    Q_REQUIRED_RESULT MessageListView::Mode mode() const;

    void setRoomId(const QString &roomID);

    void goToMessage(const QString &messageId);
    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    void copyMessageToClipboard(const QModelIndex &index = {});

    void clearTextDocumentCache();

    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;

protected:
    void resizeEvent(QResizeEvent *ev) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

Q_SIGNALS:
    void modelChanged();
    void editMessageRequested(const QString &messageId, const QString &text);
    void quoteMessageRequested(const QString &messageId, const QString &text);
    void createNewDiscussion(const QString &messageId, const QString &originalMessage);
    void createPrivateConversation(const QString &userName);
    void loadHistoryRequested();
    void goToMessageRequested(const QString &messageId, const QString &messageDateTimeUtc);
    void replyInThreadRequested(const QString &messageId, const QString &text);

private:
    void checkIfAtBottom();
    void maybeScrollToBottom();
    void slotEditMessage(const QModelIndex &index);
    void slotDeleteMessage(const QModelIndex &index);
    void slotReportMessage(const QModelIndex &index);
    void slotSetAsFavorite(const QModelIndex &index, bool isStarred);
    void slotSetPinnedMessage(const QModelIndex &index, bool isPinned);
    void slotStartDiscussion(const QModelIndex &index);
    void slotStartPrivateConversation(const QModelIndex &index);
    void slotTranslateMessage(const QModelIndex &index, bool checked);
    void createSeparator(QMenu &menu);
    void slotVerticalScrollbarChanged(int value);
    void slotReplyInThread(const QModelIndex &index);
    void updateVerticalPageStep();
    void slotDebugMessage(const QModelIndex &index);
    void slotUpdateLastSeen();
    void slotMarkMessageAsUnread(const QModelIndex &index);
    void slotSelectAll(const QModelIndex &index);
    void handleMouseEvent(QMouseEvent *event);
    void slotShowFullThread(const QModelIndex &index);
    void slotQuoteMessage(const QModelIndex &index);
    void slotCopyLinkToMessage(const QModelIndex &index);
    void slotFollowMessage(const QModelIndex &index, bool messageIsFollowing);
    Q_REQUIRED_RESULT QString generatePermalink(const QString &messageId) const;
    void slotShowUserInfo(const QString &userName);
    Q_REQUIRED_RESULT QStyleOptionViewItem listViewOptions() const;

private:
    QPointer<Room> mRoom;
    const MessageListView::Mode mMode = MessageListView::Mode::Editing;
    MessageListDelegate *const mMessageListDelegate;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QVector<PluginTextInterface *> mPluginTextInterface;
    QPoint mPressedPosition;
    bool mAtBottom = true;
    bool mDebug = false;
};

