/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#pragma once

#include "libruqolawidgets_private_export.h"
#include "utils.h"

#include "misc/pixmapcache.h"

#include <QFont>
#include <QItemDelegate>
#include <QScopedPointer>

class RocketChatAccount;
class Message;
class MessageDelegateHelperBase;
class MessageDelegateHelperText;
class MessageAttachmentDelegateHelperImage;
class MessageAttachmentDelegateHelperFile;
class MessageDelegateHelperReactions;
class MessageAttachmentDelegateHelperVideo;
class MessageAttachmentDelegateHelperSound;
class MessageAttachmentDelegateHelperText;
class MessageAttachment;
class AvatarCacheManager;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit MessageListDelegate(QObject *parent = nullptr);
    ~MessageListDelegate() override;

    void setRocketChatAccount(RocketChatAccount *rcAccount);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);
    bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    void drawDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, bool drawLastSeenLine) const;

    void setShowThreadContext(bool b);

    Q_REQUIRED_RESULT bool hasSelection() const;
    Q_REQUIRED_RESULT QString selectedText() const;

    void selectAll(const QStyleOptionViewItem &option, const QModelIndex &index);
    void clearTextDocumentCache();

private:
    Q_REQUIRED_RESULT QPixmap makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const;

    struct Layout {
        // Sender
        QString senderText;
        QFont senderFont;
        QRectF senderRect;

        // Avatar pixmap
        QPixmap avatarPixmap;
        QPointF avatarPos;

        // Roles icon
        QRect rolesIconRect;

        // Edited icon
        QRect editedIconRect;

        // Favorite icon
        QRect favoriteIconRect;

        // Pinned icon
        QRect pinIconRect;

        // Translated icon
        QRect translatedIconRect;

        // Show Ignore Message icon
        QRect showIgnoredMessageIconRect;

        // add-reaction button and timestamp
        QRect addReactionRect;
        QString timeStampText;
        QPoint timeStampPos;
        QRect timeStampRect;

        QRect usableRect; // rect for everything except the date header (at the top) and the sender (on the left)

        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp

        // Attachments
        QRect attachmentsRect;
        QVector<QRect> attachmentsRectList;

        // Reactions
        qreal reactionsY = 0;
        qreal reactionsHeight = 0;

        // Replies
        qreal repliesY = 0;
        qreal repliesHeight = 0;

        // Discussions
        qreal discussionsHeight = 0;

        // Last See
        qreal displayLastSeenMessageY = 0;

        // showIgnoreMessage
        bool showIgnoreMessage = false;
    };
    Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawLastSeenLine(QPainter *painter, qint64 displayLastSeenY, const QStyleOptionViewItem &option) const;

    /// @note Ownership is not transferred
    MessageDelegateHelperBase *attachmentsHelper(const MessageAttachment &msgAttach) const;

    friend class MessageListDelegateTest;

    const QIcon mEditedIcon;
    const QIcon mRolesIcon;
    const QIcon mAddReactionIcon;
    const QIcon mFavoriteIcon;
    const QIcon mPinIcon;
    const QIcon mTranslatedIcon;
    QColor mEditColorMode;
    QColor mOpenDiscussionColorMode;
    QColor mReplyThreadColorMode;
    RocketChatAccount *mRocketChatAccount = nullptr;

    QScopedPointer<MessageDelegateHelperText> mHelperText;
    QScopedPointer<MessageAttachmentDelegateHelperImage> mHelperAttachmentImage;
    QScopedPointer<MessageAttachmentDelegateHelperFile> mHelperAttachmentFile;
    QScopedPointer<MessageDelegateHelperReactions> mHelperReactions;
    QScopedPointer<MessageAttachmentDelegateHelperVideo> mHelperAttachmentVideo;
    QScopedPointer<MessageAttachmentDelegateHelperSound> mHelperAttachmentSound;
    QScopedPointer<MessageAttachmentDelegateHelperText> mHelperAttachmentText;
    AvatarCacheManager *const mAvatarCacheManager;
};

