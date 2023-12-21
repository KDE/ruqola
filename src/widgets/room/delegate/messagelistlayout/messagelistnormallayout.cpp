/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistnormallayout.h"
#include "delegateutils/messagedelegateutils.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messageattachmentdelegatehelperbase.h"
#include "room/delegate/messagedelegatehelperreactions.h"
#include "room/delegate/messagedelegatehelpertext.h"
#include "room/delegate/messagelistdelegate.h"

MessageListNormalLayout::MessageListNormalLayout(MessageListDelegate *delegate)
    : MessageListLayoutBase(delegate)
{
}

MessageListNormalLayout::~MessageListNormalLayout() = default;

// Use big icon 2 lines
// [Optional date header]
// [margin] <pixmap> [margin] <sender>
// [margin] <editicon> [margin] <text message> [margin] <add reaction> [margin] <timestamp> [margin/2]
//                                                                  <attachments>
//                                                                  <blocks>
//                                                                  <reactions>
//                                                                  <N replies>
MessageListLayoutBase::Layout MessageListNormalLayout::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);

    Layout layout;
    generateSenderInfo(layout, message, option, index);

    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = layout.sameSenderAsPreviousMessage ? 0 : senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);

    if (mRocketChatAccount && mRocketChatAccount->displayAvatars()) {
        layout.avatarPixmap = mDelegate->makeAvatarPixmap(option.widget, index, senderTextSize.height() * 2);
    }

    QRect usableRect = option.rect;
    const bool displayLastSeenMessage = index.data(MessagesModel::DisplayLastSeenMessage).toBool();
    if (index.data(MessagesModel::DateDiffersFromPrevious).toBool()) {
        usableRect.setTop(usableRect.top() + option.fontMetrics.height());
    } else if (displayLastSeenMessage) {
        layout.displayLastSeenMessageY = usableRect.top();
    }

    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.
    usableRect.setTop(usableRect.top() + senderAscent); // FIXME position.
    const qreal margin = MessageDelegateUtils::basicMargin();
    const int avatarWidth = MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).width();
    const int senderX = option.rect.x() + avatarWidth + 2 * margin;
    int textLeft = avatarWidth + 2 * margin;

    int positionIcon = senderX + senderTextSize.width() + margin;
    // Roles icon
    const qreal iconSizeMargin = iconSize + margin;
    const bool hasRoles = !index.data(MessagesModel::Roles).toString().isEmpty() && mRocketChatAccount && !mRocketChatAccount->hideRoles();
    if (hasRoles) {
        positionIcon += iconSizeMargin;
    }

    // Edit icon
    const int editIconX = positionIcon;
    if (message->wasEdited()) {
        positionIcon += iconSizeMargin;
    }

    const int favoriteIconX = positionIcon;
    // Favorite icon
    if (message->isStarred()) {
        positionIcon += iconSizeMargin;
    }

    const int pinIconX = positionIcon;
    // Pin icon
    if (message->isPinned()) {
        positionIcon += iconSizeMargin;
    }

    const int followingIconX = positionIcon;
    layout.messageIsFollowing = mRocketChatAccount && message->replies().contains(mRocketChatAccount->userId());
    // Following icon
    if (layout.messageIsFollowing) {
        positionIcon += iconSizeMargin;
    }

    const int translatedIconX = positionIcon;
    // translated icon
    if (message->isAutoTranslated()) {
        positionIcon += iconSizeMargin;
    }

    const int showIgnoreMessageIconX = positionIcon;
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
    int attachmentsY;
    const int textVMargin = 3; // adjust this for "compactness"
    if (textSize.isValid()) {
        layout.textRect = QRect(textLeft,
                                usableRect.top() + textVMargin + (layout.sameSenderAsPreviousMessage ? 0 : layout.senderRect.height()),
                                maxWidth,
                                textSize.height() + textVMargin);
        attachmentsY = layout.textRect.y() + layout.textRect.height();
        layout.baseLine += option.rect.top(); // make it absolute
    } else {
        attachmentsY = usableRect.top() + textVMargin;
        layout.baseLine = attachmentsY + option.fontMetrics.ascent();
    }
    layout.usableRect.setLeft(textLeft);

    // Align top of sender rect so it matches the baseline of the richtext
    layout.senderRect =
        QRectF(senderX, layout.baseLine - senderAscent, senderTextSize.width(), (layout.sameSenderAsPreviousMessage ? 0 : layout.senderRect.height()));
    if (index.data(MessagesModel::DateDiffersFromPrevious).toBool()) {
        layout.baseLine += option.fontMetrics.height();
        layout.senderRect.setTop(layout.senderRect.top() + senderAscent);
    }
    // Align top of avatar with top of sender rect
    const double senderRectY{layout.senderRect.y()};
    layout.avatarPos = QPointF(option.rect.x() + margin, senderRectY);
    // Same for the roles and edit icon
    if (hasRoles) {
        layout.rolesIconRect = QRect(editIconX - iconSize - margin, senderRectY, iconSize, iconSize);
    }
    const int iconPosition = usableRect.top();

    int iconIndex = 0;
    if (message->wasEdited()) {
        if (layout.sameSenderAsPreviousMessage) {
            layout.editedIconRect = iconRect(iconIndex, senderX, iconPosition, iconSize, margin, avatarWidth);
            iconIndex++;
        } else {
            layout.editedIconRect = QRect(editIconX, senderRectY, iconSize, iconSize);
        }
    }

    if (message->isStarred()) {
        if (layout.sameSenderAsPreviousMessage) {
            layout.favoriteIconRect = iconRect(iconIndex, senderX, iconPosition, iconSize, margin, avatarWidth);
            iconIndex++;
        } else {
            layout.favoriteIconRect = QRect(favoriteIconX, senderRectY, iconSize, iconSize);
        }
    }

    if (message->isPinned()) {
        if (layout.sameSenderAsPreviousMessage) {
            layout.pinIconRect = iconRect(iconIndex, senderX, iconPosition, iconSize, margin, avatarWidth);
            iconIndex++;
        } else {
            layout.pinIconRect = QRect(pinIconX, senderRectY, iconSize, iconSize);
        }
    }
    if (layout.messageIsFollowing) {
        if (layout.sameSenderAsPreviousMessage) {
            layout.followingIconRect = iconRect(iconIndex, senderX, iconPosition, iconSize, margin, avatarWidth);
            iconIndex++;
        } else {
            layout.followingIconRect = QRect(followingIconX, senderRectY, iconSize, iconSize);
        }
    }
    if (message->isAutoTranslated()) {
        if (layout.sameSenderAsPreviousMessage) {
            layout.translatedIconRect = iconRect(iconIndex, senderX, iconPosition, iconSize, margin, avatarWidth);
            // iconIndex++;
        } else {
            layout.translatedIconRect = QRect(translatedIconX, senderRectY, iconSize, iconSize);
        }
    }

    if (ignoreMessage) {
        layout.showIgnoredMessageIconRect = QRect(showIgnoreMessageIconX, senderRectY, iconSize, iconSize);
        layout.showIgnoreMessage = index.data(MessagesModel::ShowIgnoredMessage).toBool();
    }

    layout.addReactionRect = QRect(textLeft + textSize.width() + margin, layout.textRect.y(), iconSize, iconSize);
    if (layout.sameSenderAsPreviousMessage) {
        layout.addReactionRect.moveTop(layout.textRect.y());
    }
    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);
    layout.timeStampRect = QRect(QPoint(layout.timeStampPos.x(), usableRect.top()), timeSize);
    generateAttachmentLayout(mDelegate, layout, message, attachmentsY, textLeft, maxWidth, option, index);
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
    // Increase size when we have more than 2 icons
    if (layout.sameSenderAsPreviousMessage) {
        if (iconIndex > 2) {
            layout.discussionsHeight += margin;
        }
        if (iconIndex > 4) {
            layout.discussionsHeight += iconSize;
        }
    }
    return layout;
}

QRect MessageListNormalLayout::iconRect(int iconIndex, int senderX, int iconPosition, int iconSize, int margin, int avatarWidth) const
{
    switch (iconIndex) {
    case 0:
        return QRect(senderX - margin - avatarWidth / 2, iconPosition, iconSize, iconSize);
    case 1:
        return QRect(senderX - margin - avatarWidth, iconPosition, iconSize, iconSize);
    case 2:
        return QRect(senderX - margin - avatarWidth, iconPosition + iconSize, iconSize, iconSize);
    case 3:
        return QRect(senderX - margin - avatarWidth / 2, iconPosition + iconSize, iconSize, iconSize);
    case 4:
        return QRect(senderX - margin - avatarWidth / 2, iconPosition + 2 * iconSize, iconSize, iconSize);
    }
    Q_UNREACHABLE();
    return {};
}

QSize MessageListNormalLayout::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const MessageListLayoutBase::Layout layout = doLayout(option, index);

    int additionalHeight = 5;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 10; // Add more space as cozy mode
    }

    // contents is date + text + attachments + reactions + replies + discussions (where all of those are optional)
    const int contentsHeight = layout.repliesY + layout.repliesHeight + layout.discussionsHeight - option.rect.y();
    const int senderAndAvatarHeight = qMax<int>(
        (layout.sameSenderAsPreviousMessage ? 0 : layout.senderRect.y()) + (layout.sameSenderAsPreviousMessage ? 0 : layout.senderRect.height())
            - option.rect.y(),
        (layout.sameSenderAsPreviousMessage ? 0 : layout.avatarPos.y() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).height()) - option.rect.y());

    // qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height()
    //         << "attachments" << layout.attachmentsRect.height() << "reactions" << layout.reactionsHeight << "total contents" << contentsHeight;
    // qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    const QSize size = {option.rect.width(), qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight};
    return size;
}
