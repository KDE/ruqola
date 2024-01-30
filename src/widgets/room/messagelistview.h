/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
#include "moderation/moderationreportinfos.h"
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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotEditMessage(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeleteMessage(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotReportMessage(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSetAsFavorite(const QModelIndex &index, bool isStarred);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSetPinnedMessage(const QModelIndex &index, bool isPinned);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotStartDiscussion(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotStartPrivateConversation(const QString &userName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTranslateMessage(const QModelIndex &index, bool checked);
    LIBRUQOLAWIDGETS_NO_EXPORT void createSeparator(QMenu &menu);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVerticalScrollbarChanged(int value);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotReplyInThread(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDebugMessage(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMarkMessageAsUnread(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectAll(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowFullThread(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotQuoteMessage(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCopyLinkToMessage(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFollowMessage(const QModelIndex &index, bool messageIsFollowing);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString generatePermalink(const QString &messageId) const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowUserInfo(const QString &userName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateView();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTranslate(const QString &from, const QString &to, const QPersistentModelIndex &modelIndex);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextToSpeech(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void addDebugMenu(QMenu &menu, const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void addSelectedMessageBackgroundAnimation(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void createTranslorMenu();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowReportInfo(const ModerationReportInfos &info);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString selectedText(const QModelIndex &index) override;
    QPointer<Room> mRoom;
    const MessageListView::Mode mMode = MessageListView::Mode::Editing;
    MessageListDelegate *const mMessageListDelegate;
    TextTranslator::TranslatorMenu *mTranslatorMenu = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
