/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistcompactlayout.h"
#include "delegateutils/messagedelegateutils.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messageattachmentdelegatehelperbase.h"
#include "room/delegate/messageblockdelegatehelperbase.h"
#include "room/delegate/messagedelegatehelperreactions.h"
#include "room/delegate/messagedelegatehelpertext.h"
#include "room/delegate/messagelistdelegate.h"

MessageListCompactLayout::MessageListCompactLayout(MessageListDelegate *delegate)
    : MessageListLayoutBase(delegate)
{
}

MessageListCompactLayout::~MessageListCompactLayout() = default;

// [Optional date header]
// [margin] <pixmap> [margin] <sender> [margin] <editicon> [margin] <text message> [margin] <add reaction> [margin] <timestamp> [margin/2]
//                                                                  <attachments>
//                                                                  <blocks>
//                                                                  <reactions>
//                                                                  <N replies>
MessageListLayoutBase::Layout MessageListCompactLayout::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);

    Layout layout;
    generateSenderInfo(layout, message, option, index);

    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);

    if (mRocketChatAccount && mRocketChatAccount->displayAvatars()) {
        layout.avatarPixmap = mDelegate->makeAvatarPixmap(option.widget, index, senderTextSize.height());
    }

    QRect usableRect = option.rect;
    const bool displayLastSeenMessage = index.data(MessagesModel::DisplayLastSeenMessage).toBool();
    if (index.data(MessagesModel::DateDiffersFromPrevious).toBool()) {
        usableRect.setTop(usableRect.top() + option.fontMetrics.height());
    } else if (displayLastSeenMessage) {
        layout.displayLastSeenMessageY = usableRect.top();
    }

    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    const qreal margin = MessageDelegateUtils::basicMargin();
    const int senderX = option.rect.x() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).width() + 2 * margin;
    int textLeft = senderX + senderTextSize.width() + margin;

    const qreal iconSizeMargin = iconSize + margin;
    // Roles icon
    const bool hasRoles = !index.data(MessagesModel::Roles).toString().isEmpty() && mRocketChatAccount && !mRocketChatAccount->hideRoles();
    if (hasRoles) {
        textLeft += iconSizeMargin;
    }

    // Edit icon
    const int editIconX = textLeft;
    if (message->wasEdited()) {
        textLeft += iconSizeMargin;
    }

    const int favoriteIconX = textLeft;
    // Favorite icon
    if (message->isStarred()) {
        textLeft += iconSizeMargin;
    }

    const int pinIconX = textLeft;
    // Pin icon
    if (message->isPinned()) {
        textLeft += iconSizeMargin;
    }

    const int encryptionX = textLeft;
    // Encruption icon
    if (message->isEncryptedMessage()) {
        textLeft += iconSizeMargin;
    }

    const int followingIconX = textLeft;
    layout.messageIsFollowing = mRocketChatAccount && message->replies() && message->replies()->replies().contains(mRocketChatAccount->userId());
    // Following icon
    if (layout.messageIsFollowing) {
        textLeft += iconSizeMargin;
    }

    const int translatedIconX = textLeft;
    // translated icon
    if (message->isAutoTranslated() || !message->localTranslation().isEmpty()) {
        textLeft += iconSizeMargin;
    }

    const int showIgnoreMessageIconX = textLeft;
    // showIgnoreMessage icon
    const bool ignoreMessage = MessageDelegateUtils::showIgnoreMessages(index);
    if (ignoreMessage) {
        textLeft += iconSizeMargin;
    }

    // Timestamp
    layout.timeStampText = index.data(MessagesModel::Timestamp).toString();
    const QSize timeSize = MessageDelegateUtils::timeStampSize(layout.timeStampText, option);

    // Message (using the rest of the available width)
    const int widthAfterMessage = iconSizeMargin + timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);
    layout.baseLine = 0;
    const QSize textSize = mDelegate->helperText()->sizeHint(index, maxWidth, option, &layout.baseLine);
    int attachmentsY = 0;
    const int textVMargin = 3; // adjust this for "compactness"
    if (textSize.isValid()) {
        layout.textRect = QRect(textLeft, usableRect.top() + textVMargin, maxWidth, textSize.height() + textVMargin);
        attachmentsY = layout.textRect.y() + layout.textRect.height();
        layout.baseLine += layout.textRect.top(); // make it absolute
    } else {
        attachmentsY = usableRect.top() + textVMargin;
        layout.baseLine = attachmentsY + option.fontMetrics.ascent();
    }
    layout.usableRect.setLeft(textLeft);

    // Align top of sender rect so it matches the baseline of the richtext
    layout.senderRect = QRectF(senderX, layout.baseLine - senderAscent, senderTextSize.width(), senderTextSize.height());
    // Align top of avatar with top of sender rect
    const double senderRectY{layout.senderRect.y()};
    layout.avatarPos = QPointF(option.rect.x() + margin, senderRectY);
    // Same for the roles and edit icon
    if (hasRoles) {
        layout.rolesIconRect = QRect(editIconX - iconSize - margin, senderRectY, iconSize, iconSize);
    }
    if (message->wasEdited()) {
        layout.editedIconRect = QRect(editIconX, senderRectY, iconSize, iconSize);
    }

    if (message->isStarred()) {
        layout.favoriteIconRect = QRect(favoriteIconX, senderRectY, iconSize, iconSize);
    }

    if (message->isPinned()) {
        layout.pinIconRect = QRect(pinIconX, senderRectY, iconSize, iconSize);
    }
    if (layout.messageIsFollowing) {
        layout.followingIconRect = QRect(followingIconX, senderRectY, iconSize, iconSize);
    }
    if (message->isAutoTranslated() || !message->localTranslation().isEmpty()) {
        layout.translatedIconRect = QRect(translatedIconX, senderRectY, iconSize, iconSize);
    }

    if (message->isEncryptedMessage()) {
        layout.encryptedIconRect = QRect(encryptionX, senderRectY, iconSize, iconSize);
    }

    if (ignoreMessage) {
        layout.showIgnoredMessageIconRect = QRect(showIgnoreMessageIconX, senderRectY, iconSize, iconSize);
        layout.showIgnoreMessage = index.data(MessagesModel::ShowIgnoredMessage).toBool();
    }
    layout.addReactionRect = QRect(textLeft + textSize.width() + margin, senderRectY, iconSize, iconSize);
    layout.replyToThreadRect = QRect(textLeft + textSize.width() + 2 * margin + iconSize, senderRectY, iconSize, iconSize);

    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);
    layout.timeStampRect = QRect(QPoint(layout.timeStampPos.x(), senderRectY), timeSize);

    generateAttachmentBlockAndUrlPreviewLayout(mDelegate, layout, message, attachmentsY, textLeft, maxWidth, option, index);
    layout.reactionsHeight = mDelegate->helperReactions()->sizeHint(index, maxWidth, option).height();

    // Replies
    layout.repliesY = layout.reactionsY + layout.reactionsHeight;
    if (message->threadCount() > 0) {
        layout.repliesHeight = option.fontMetrics.height();
    }
    // Discussions
    if (!message->discussionRoomId().isEmpty()) {
        layout.discussionsHeight = option.fontMetrics.height();
    }

    return layout;
}

QSize MessageListCompactLayout::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const MessageListLayoutBase::Layout layout = doLayout(option, index);

    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }

    // contents is date + text + attachments + reactions + replies + discussions (where all of those are optional)
    const int contentsHeight = layout.repliesY + layout.repliesHeight + layout.discussionsHeight - option.rect.y();
    const int senderAndAvatarHeight = qMax<int>(layout.senderRect.y() + layout.senderRect.height() - option.rect.y(),
                                                layout.avatarPos.y() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).height() - option.rect.y());

    // qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height()
    //         << "attachments" << layout.attachmentsRect.height() << "reactions" << layout.reactionsHeight << "total contents" << contentsHeight;
    // qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    const QSize size = {option.rect.width(), qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight};
    return size;
}
