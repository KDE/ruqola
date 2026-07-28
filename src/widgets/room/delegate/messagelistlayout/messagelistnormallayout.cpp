/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistnormallayout.h"
#include "config-ruqola.h"
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
    const bool dateDiffersFromPrevious = index.data(MessagesModel::DateDiffersFromPrevious).toBool();
    // Empty space above the message block that sets the vertical rhythm: a full gap when the
    // author changes (so a new speaker reads as a new block) and a small one for a grouped
    // consecutive message from the same author. Deliberately larger than the name-to-text gap
    // (textVMargin) further down, so proximity groups each author with their own text.
    const int blockTopSpacing = layout.sameSenderAsPreviousMessage ? MessageDelegateUtils::groupedMessageSpacing() : MessageDelegateUtils::senderBlockSpacing();

    // A date header and a standalone unread-messages line each occupy a band at the top of
    // the row. Reserve it here; the author line is shifted down by the same amount below.
    int topBandHeight = 0;
    if (dateDiffersFromPrevious) {
        topBandHeight = option.fontMetrics.height();
    } else if (displayLastSeenMessage) {
        topBandHeight = option.fontMetrics.height();
        // Center the line in its band so it gets symmetric padding instead of hugging the
        // top of the next message.
        layout.displayLastSeenMessageY = usableRect.top() + topBandHeight / 2;
    }
    usableRect.setTop(usableRect.top() + topBandHeight + blockTopSpacing);

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

    const int encryptionX = positionIcon;
    // Encruption icon
    if (message->isEncryptedMessage()) {
        positionIcon += iconSizeMargin;
    }

    const int followingIconX = positionIcon;
    layout.messageIsFollowing = mRocketChatAccount && (message->replies() && message->replies()->replies().contains(mRocketChatAccount->userId()));
    // Following icon
    if (layout.messageIsFollowing) {
        positionIcon += iconSizeMargin;
    }

    const int translatedIconX = positionIcon;
    // translated icon
    if (message->isAutoTranslated() || !message->localTranslation().isEmpty()) {
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

    // A grouped row shows its hover timestamp in the avatar gutter, but falls back to the
    // right edge when there is no gutter (avatars off) or the gutter is already taken by
    // status icons (edited/starred/…). Compute that here since the width reservation below
    // depends on it, and the status icons are only laid out further down (mirror them).
    const bool groupedStatusIconsInGutter = layout.sameSenderAsPreviousMessage
        && (message->wasEdited() || message->isStarred() || message->isPinned() || layout.messageIsFollowing || message->isEncryptedMessage()
            || message->isAutoTranslated() || !message->localTranslation().isEmpty());
    const bool timeStampUsesRightEdge = layout.sameSenderAsPreviousMessage && (avatarWidth < timeSize.width() || groupedStatusIconsInGutter);

    // Message (using the rest of the available width). Reserve room after the text for the
    // trailing hover-action icons (add-reaction, reply-in-thread, and text-to-speech when
    // built) so they stay on the row; the timestamp itself moved to the author line and no
    // longer needs right-edge space, except for a grouped row that uses the right-edge
    // fallback, where its width is reserved so it cannot overprint a long line.
    qreal hoverActionsWidth = 2 * iconSizeMargin; // add-reaction + reply-in-thread
#if HAVE_TEXT_TO_SPEECH
    hoverActionsWidth += iconSizeMargin; // text-to-speech
#endif
    const int widthAfterMessage = hoverActionsWidth + margin / 2 + (timeStampUsesRightEdge ? timeSize.width() + margin : 0);
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
        QRectF(senderX, layout.baseLine - senderAscent, senderTextSize.width(), (layout.sameSenderAsPreviousMessage ? 0 : senderTextSize.height()));
    // usableRect already pushed the message text down by the top band (a date header drawn by
    // drawDate, or a standalone unread-messages line) plus the block-top spacing. Shift the whole
    // author line — the name baseline, its rect, and therefore the avatar — down by the same
    // amount, so the name stays exactly one line above its own text regardless of that offset.
    // Only the text branch needs this: it derived baseLine from option.rect.top() (unshifted).
    // The empty-text branch (attachment/blocks/urls only) already derived baseLine from
    // usableRect.top(), which includes both offsets, so shifting again would double-count and
    // drop the author line below its own attachment.
    const int authorLineShift = topBandHeight + blockTopSpacing;
    if (textSize.isValid() && authorLineShift > 0) {
        layout.baseLine += authorLineShift;
        layout.senderRect.moveTop(layout.senderRect.top() + authorLineShift);
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

    if (message->isEncryptedMessage()) {
        if (layout.sameSenderAsPreviousMessage) {
            layout.encryptedIconRect = iconRect(iconIndex, senderX, iconPosition, iconSize, margin, avatarWidth);
            iconIndex++;
        } else {
            layout.encryptedIconRect = QRect(encryptionX, senderRectY, iconSize, iconSize);
        }
    }

    if (message->isAutoTranslated() || !message->localTranslation().isEmpty()) {
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
    layout.replyToThreadRect = QRect(textLeft + textSize.width() + 2 * margin + iconSize, layout.textRect.y(), iconSize, iconSize);
    if (layout.sameSenderAsPreviousMessage) {
        layout.addReactionRect.moveTop(layout.textRect.y());
    }
#if HAVE_TEXT_TO_SPEECH
    layout.textToSpeechIconRect = QRect(textLeft + textSize.width() + 3 * margin + iconSize * 2, layout.textRect.y(), iconSize, iconSize);
#endif

    // Right edge available to laid-out content (a half-margin gutter is kept clear).
    const int rightEdge = option.rect.width() - margin / 2;
    if (!layout.sameSenderAsPreviousMessage) {
        // Group the time with the author line, right after the sender name (and any
        // author-line icons): "Alice Martin · 12:34 ✓✓". The old far-right placement
        // stranded it ~a column width from the text it belonged to.
        const QString separator = QStringLiteral("·  "); // middot
        layout.timeStampText = separator + layout.timeStampText;
        const QSize authorTimeSize = MessageDelegateUtils::timeStampSize(layout.timeStampText, option);
        // Start just after the sender name and its author-line icons. The ignored-message
        // icon advances textLeft rather than positionIcon, so step past it explicitly.
        int timeX = positionIcon;
        if (ignoreMessage) {
            timeX += iconSizeMargin;
        }
        // Keep the time and its read receipt inside the row: a very long display name or a
        // pile of author-line icons could otherwise push them past the right edge (the old
        // fixed-right placement was always visible). Clamp so both stay on screen.
        const int rightLimit = rightEdge - iconSize - margin - authorTimeSize.width();
        timeX = qMin(timeX, rightLimit);
        layout.timeStampPos = QPoint(timeX, layout.baseLine);
        layout.timeStampRect = QRect(QPoint(timeX, senderRectY), authorTimeSize);
        layout.readReceiptIconRect = QRect(layout.timeStampRect.right() + margin, senderRectY, iconSize, iconSize);
    } else {
        // Grouped consecutive message: no author line, so the delegate draws the time on
        // hover only (see paint()), aligned to the first content line. Preferred spot is
        // the empty avatar gutter (Slack-style); when that gutter is unavailable or already
        // holds status icons, fall back to the right edge (maxWidth reserves its width).
        layout.timeStampHoverOnly = true;
        const int contentTop = layout.textRect.isValid() ? layout.textRect.y() : attachmentsY;
        const int gutterRight = textLeft - margin;
        const int timeX = timeStampUsesRightEdge ? rightEdge - timeSize.width() // right edge fallback
                                                 : gutterRight - timeSize.width(); // right-aligned in the avatar gutter
        layout.timeStampPos = QPoint(timeX, contentTop + option.fontMetrics.ascent());
        layout.timeStampRect = QRect(timeX, contentTop, timeSize.width(), option.fontMetrics.height());
        // No per-message read receipt on grouped rows (it would strand a tiny check next
        // to the hover time); the receipt stays with the author line above.
        layout.readReceiptIconRect = QRect();
    }
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
    case 5:
        return QRect(senderX - margin - avatarWidth, iconPosition + 3 * iconSize, iconSize, iconSize);
    default:
        break;
    }
    Q_UNREACHABLE();
    return {};
}

QSize MessageListNormalLayout::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const MessageListLayoutBase::Layout layout = doLayout(option, index);

    // Most inter-message separation now lives in the block-top spacing (added in doLayout to the
    // top of the next block), so only a small breather is needed under each row to keep the two
    // gaps from stacking into an oversized space between messages.
    int additionalHeight = 2;
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
