/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messagelistlayout/messagelistlayoutbase.h"
#include "room.h"

#include "lrucache.h"
#include <QItemDelegate>
#include <QScopedPointer>

class QListView;
class RocketChatAccount;
class Message;
class MessageAttachmentDelegateHelperBase;
class MessageDelegateHelperText;
class MessageAttachmentDelegateHelperImage;
class MessageAttachmentDelegateHelperFile;
class MessageDelegateHelperReactions;
class MessageAttachmentDelegateHelperVideo;
class MessageAttachmentDelegateHelperSound;
class MessageAttachmentDelegateHelperText;
class MessageDelegateHelperConferenceVideo;
class MessageDelegateHelperUrlPreview;
class MessageAttachment;
class AvatarCacheManager;
class TextSelectionImpl;
class MessageBlockDelegateHelperBase;
class Block;
class QMenu;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    struct MenuInfo {
        Room::RoomType roomType = Room::RoomType::Unknown;
        QPoint globalPos;
        QPoint pos;
        bool editMode = false;
    };

    explicit MessageListDelegate(RocketChatAccount *account, QListView *view);
    ~MessageListDelegate() override;

    void setRocketChatAccount(RocketChatAccount *rcAccount);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);
    bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    void drawDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, bool drawLastSeenLine) const;

    void setShowThreadContext(bool b);

    [[nodiscard]] bool hasSelection() const;
    [[nodiscard]] QString selectedText() const;

    void selectAll(const QStyleOptionViewItem &option, const QModelIndex &index);
    void clearTextDocumentCache();

    void clearSelection();

    [[nodiscard]] QString urlAt(const QStyleOptionViewItem &option, const QModelIndex &index, QPoint pos) const;

    [[nodiscard]] bool contextMenu(const QStyleOptionViewItem &option, const QModelIndex &index, const MessageListDelegate::MenuInfo &info);

    void attachmentContextMenu(const QStyleOptionViewItem &option, const QModelIndex &index, const MessageListDelegate::MenuInfo &info, QMenu *menu);

    void setSearchText(const QString &newSearchText);

    [[nodiscard]] QPixmap makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const;

    /// @note Ownership is not transferred
    [[nodiscard]] MessageAttachmentDelegateHelperBase *attachmentsHelper(const MessageAttachment &msgAttach) const;

    [[nodiscard]] MessageBlockDelegateHelperBase *blocksHelper(const Block &block) const;

    [[nodiscard]] MessageDelegateHelperText *helperText() const;

    [[nodiscard]] MessageDelegateHelperReactions *helperReactions() const;

    [[nodiscard]] MessageDelegateHelperUrlPreview *helperUrlPreview() const;

    void removeMessageCache(const Message *message);

    void setEnableEmojiMenu(bool b);

    void clearSizeHintCache();

    void removeSizeHintCache(const QString &messageId);

Q_SIGNALS:
    void showUserInfo(const QString &userName);
    void startPrivateConversation(const QString &userName);
    void updateView();

private:
    void slotUpdateColors();
    void switchMessageLayout();
    void slotPrivateSettingsChanged();
    void clearAvatarSizeHintCache();

    [[nodiscard]] MessageListLayoutBase::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawLastSeenLine(QPainter *painter, qint64 displayLastSeenY, const QStyleOptionViewItem &option) const;
    void drawModerationDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, const QString &roomName) const;
    [[nodiscard]] bool isSystemMessage(const Message *message) const;
    [[nodiscard]] QString cacheIdentifier(const QModelIndex &index) const;

    friend class MessageListDelegateTest;

    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QString, QSize> mSizeHintCache;

    const QIcon mEditedIcon;
    const QIcon mRolesIcon;
    const QIcon mAddReactionIcon;
    const QIcon mFavoriteIcon;
    const QIcon mFollowingIcon;
    const QIcon mPinIcon;
    const QIcon mTranslatedIcon;
    QColor mEditColorMode;
    QColor mThreadedMessageBackgroundColor;
    QColor mOpenDiscussionColorMode;
    QColor mReplyThreadColorMode;
    QColor mHoverHightlightColor;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QListView *const mListView;
    TextSelectionImpl *const mTextSelectionImpl;

    QScopedPointer<MessageDelegateHelperText> mHelperText;
    QScopedPointer<MessageAttachmentDelegateHelperImage> mHelperAttachmentImage;
    QScopedPointer<MessageAttachmentDelegateHelperFile> mHelperAttachmentFile;
    QScopedPointer<MessageDelegateHelperReactions> mHelperReactions;
    QScopedPointer<MessageAttachmentDelegateHelperVideo> mHelperAttachmentVideo;
    QScopedPointer<MessageAttachmentDelegateHelperSound> mHelperAttachmentSound;
    QScopedPointer<MessageAttachmentDelegateHelperText> mHelperAttachmentText;
    QScopedPointer<MessageDelegateHelperConferenceVideo> mHelperConferenceVideo;
    QScopedPointer<MessageDelegateHelperUrlPreview> mHelperUrlPreview;
    AvatarCacheManager *const mAvatarCacheManager;
    MessageListLayoutBase *mMessageListLayoutBase = nullptr;
    bool mEmojiMenuEnabled = true;
    bool mPreviewEmbed = true;
};
