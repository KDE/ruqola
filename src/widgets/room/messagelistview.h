/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
#include <QPointer>
class MessageListDelegate;
class RocketChatAccount;
class Room;
namespace TextTranslator
{
class TranslatorMenu;
}
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListView : public MessageListViewBase
{
    Q_OBJECT
public:
    enum class Mode {
        ThreadEditing,
        Editing,
        Viewing, /*when we show list of message as mentions, search etc.*/
        Moderation,
    };

    explicit MessageListView(RocketChatAccount *account, MessageListView::Mode mode, QWidget *parent = nullptr);
    ~MessageListView() override;

    void setRoom(Room *room);

    void setChannelSelected(Room *room);

    void setModel(QAbstractItemModel *newModel) override;

    void handleKeyPressEvent(QKeyEvent *ev);

    [[nodiscard]] MessageListView::Mode mode() const;

    void setRoomId(const QString &roomID);

    void goToMessage(const QString &messageId);
    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    void clearTextDocumentCache();

    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;

    void setSearchText(const QString &str);

protected:
    void paintEvent(QPaintEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void wheelEvent(QWheelEvent *e) override;

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
    void slotEditMessage(const QModelIndex &index);
    void slotDeleteMessage(const QModelIndex &index);
    void slotReportMessage(const QModelIndex &index);
    void slotSetAsFavorite(const QModelIndex &index, bool isStarred);
    void slotSetPinnedMessage(const QModelIndex &index, bool isPinned);
    void slotStartDiscussion(const QModelIndex &index);
    void slotStartPrivateConversation(const QString &userName);
    void slotTranslateMessage(const QModelIndex &index, bool checked);
    void createSeparator(QMenu &menu);
    void slotVerticalScrollbarChanged(int value);
    void slotReplyInThread(const QModelIndex &index);
    void slotDebugMessage(const QModelIndex &index);
    void slotUpdateLastSeen();
    void slotMarkMessageAsUnread(const QModelIndex &index);
    void slotSelectAll(const QModelIndex &index);
    void slotShowFullThread(const QModelIndex &index);
    void slotQuoteMessage(const QModelIndex &index);
    void slotCopyLinkToMessage(const QModelIndex &index);
    void slotFollowMessage(const QModelIndex &index, bool messageIsFollowing);
    [[nodiscard]] QString generatePermalink(const QString &messageId) const;
    void slotShowUserInfo(const QString &userName);
    void slotUpdateView();
    void slotTranslate(const QString &from, const QString &to, const QPersistentModelIndex &modelIndex);
    void slotTextToSpeech(const QModelIndex &index);

private:
    void addDebugMenu(QMenu &menu, const QModelIndex &index);
    void addSelectedMessageBackgroundAnimation(const QModelIndex &index);
    void createTranslorMenu();
    [[nodiscard]] QString selectedText(const QModelIndex &index) override;
    QPointer<Room> mRoom;
    const MessageListView::Mode mMode = MessageListView::Mode::Editing;
    MessageListDelegate *const mMessageListDelegate;
    TextTranslator::TranslatorMenu *mTranslatorMenu = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
