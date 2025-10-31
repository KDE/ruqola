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
class MessageDelegateHelperSection;
class MessageDelegateHelperActions;
class MessageDelegateHelperDivider;
class MessageDelegateHelperContext;
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
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

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

    void removeSizeHintCache(const QByteArray &messageId);

    void needUpdateIndexBackground(const QPersistentModelIndex &index, const QColor &color);
    void removeNeedUpdateIndexBackground(const QPersistentModelIndex &index);

Q_SIGNALS:
    void showUserInfo(const QString &userName);
    void startPrivateConversation(const QString &userName);
    void updateView();
    void replyToThread(const QByteArray &messageId, const QString &threadPreview);
    void translateMessage(const QModelIndex &index, bool state);

private:
    struct IndexBackgroundColor {
        QPersistentModelIndex index;
        QColor color;
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateColors();
    LIBRUQOLAWIDGETS_NO_EXPORT void switchMessageLayout();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPrivateSettingsChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void clearAvatarSizeHintCache();

    [[nodiscard]] MessageListLayoutBase::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    LIBRUQOLAWIDGETS_NO_EXPORT void drawLastSeenLine(QPainter *painter, qint64 displayLastSeenY, const QStyleOptionViewItem &option) const;
    LIBRUQOLAWIDGETS_NO_EXPORT void
    drawModerationDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, const QString &roomName) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool isSystemMessage(const Message *message) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QByteArray cacheIdentifier(const QModelIndex &index) const;

    friend class MessageListDelegateTest;

    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QByteArray, QSize> mSizeHintCache;

    const QIcon mEditedIcon;
    const QIcon mRolesIcon;
    const QIcon mAddReactionIcon;
    const QIcon mFavoriteIcon;
    const QIcon mFollowingIcon;
    const QIcon mPinIcon;
    const QIcon mTranslatedIcon;
    const QIcon mReplyInThreadIcon;
    const QIcon mEncryptedIcon;
    const QIcon mTextToSpeechIcon;
    QColor mTranslatedMessageColor;
    QColor mEditColorMode;
    QColor mThreadedMessageBackgroundColor;
    QColor mOpenDiscussionColorMode;
    QColor mReplyThreadColorMode;
    QColor mHoverHightlightColor;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QListView *const mListView;
    TextSelectionImpl *const mTextSelectionImpl;
    QList<IndexBackgroundColor> mIndexBackgroundColorList;

    QScopedPointer<MessageDelegateHelperText> mHelperText;
    QScopedPointer<MessageAttachmentDelegateHelperImage> mHelperAttachmentImage;
    QScopedPointer<MessageAttachmentDelegateHelperFile> mHelperAttachmentFile;
    QScopedPointer<MessageDelegateHelperReactions> mHelperReactions;
    QScopedPointer<MessageAttachmentDelegateHelperVideo> mHelperAttachmentVideo;
    QScopedPointer<MessageAttachmentDelegateHelperSound> mHelperAttachmentSound;
    QScopedPointer<MessageAttachmentDelegateHelperText> mHelperAttachmentText;
    QScopedPointer<MessageDelegateHelperConferenceVideo> mHelperConferenceVideo;
    QScopedPointer<MessageDelegateHelperActions> mHelperActions;
    QScopedPointer<MessageDelegateHelperSection> mHelperSection;
    QScopedPointer<MessageDelegateHelperUrlPreview> mHelperUrlPreview;
    QScopedPointer<MessageDelegateHelperDivider> mHelperDivider;
    QScopedPointer<MessageDelegateHelperContext> mHelperContext;
    AvatarCacheManager *const mAvatarCacheManager;
    MessageListLayoutBase *mMessageListLayoutBase = nullptr;
    bool mEmojiMenuEnabled = true;
    bool mPreviewEmbed = true;
};
