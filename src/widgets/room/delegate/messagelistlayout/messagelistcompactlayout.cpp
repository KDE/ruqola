/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
    if (mRocketChatAccount) {
        layout.senderText = QLatin1Char('@') + (mRocketChatAccount->useRealName() && !message->name().isEmpty() ? message->name() : message->username());
    } else {
        layout.senderText = QLatin1Char('@') + message->username();
    }
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    layout.sameSenderAsPreviousMessage = sameSenderAsPreviousMessage(index, message);
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

    const int followingIconX = textLeft;
    layout.messageIsFollowing = mRocketChatAccount && message->replies().contains(mRocketChatAccount->userId());
    // Following icon
    if (layout.messageIsFollowing) {
        textLeft += iconSizeMargin;
    }

    const int translatedIconX = textLeft;
    // translated icon
    if (message->isAutoTranslated()) {
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
    layout.avatarPos = QPointF(option.rect.x() + margin, layout.senderRect.y());
    // Same for the roles and edit icon
    if (hasRoles) {
        layout.rolesIconRect = QRect(editIconX - iconSize - margin, layout.senderRect.y(), iconSize, iconSize);
    }
    if (message->wasEdited()) {
        layout.editedIconRect = QRect(editIconX, layout.senderRect.y(), iconSize, iconSize);
    }

    if (message->isStarred()) {
        layout.favoriteIconRect = QRect(favoriteIconX, layout.senderRect.y(), iconSize, iconSize);
    }

    if (message->isPinned()) {
        layout.pinIconRect = QRect(pinIconX, layout.senderRect.y(), iconSize, iconSize);
    }
    if (layout.messageIsFollowing) {
        layout.followingIconRect = QRect(followingIconX, layout.senderRect.y(), iconSize, iconSize);
    }
    if (message->isAutoTranslated()) {
        layout.translatedIconRect = QRect(translatedIconX, layout.senderRect.y(), iconSize, iconSize);
    }

    if (ignoreMessage) {
        layout.showIgnoredMessageIconRect = QRect(showIgnoreMessageIconX, layout.senderRect.y(), iconSize, iconSize);
        layout.showIgnoreMessage = index.data(MessagesModel::ShowIgnoredMessage).toBool();
    }
    layout.addReactionRect = QRect(textLeft + textSize.width() + margin, layout.senderRect.y(), iconSize, iconSize);
    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);
    layout.timeStampRect = QRect(QPoint(layout.timeStampPos.x(), usableRect.top()), timeSize);

    if (message->attachments().isEmpty() && message->blocks().isEmpty()) {
        layout.reactionsY = attachmentsY;
    } else {
        int topAttachment = attachmentsY;
        if (!message->attachments().isEmpty()) {
            const auto attachments = message->attachments();
            QSize attachmentsSize;
            // TODO add spacing between attachment
            for (const MessageAttachment &msgAttach : attachments) {
                const MessageAttachmentDelegateHelperBase *helper = mDelegate->attachmentsHelper(msgAttach);
                if (attachmentsSize.isEmpty()) {
                    attachmentsSize = helper ? helper->sizeHint(msgAttach, index, maxWidth, option) : QSize(0, 0);
                    layout.attachmentsRectList.append(QRect(layout.senderRect.x(), topAttachment, attachmentsSize.width(), attachmentsSize.height()));
                    topAttachment += attachmentsSize.height();
                } else {
                    const QSize attSize = helper ? helper->sizeHint(msgAttach, index, maxWidth, option) : QSize(0, 0);
                    layout.attachmentsRectList.append(QRect(layout.senderRect.x(), topAttachment, attSize.width(), attSize.height()));
                    attachmentsSize = QSize(qMax(attachmentsSize.width(), attSize.width()), attSize.height() + attachmentsSize.height());
                    topAttachment += attSize.height();
                }
            }
            layout.attachmentsRect = QRect(textLeft, attachmentsY, attachmentsSize.width(), attachmentsSize.height());
        }
        if (!message->blocks().isEmpty()) {
            const auto blocks = message->blocks();
            QSize blocksSize;
            int topBlock = topAttachment;
            for (const Block &block : blocks) {
                const MessageBlockDelegateHelperBase *helper = mDelegate->blocksHelper(block);
                if (blocksSize.isEmpty()) {
                    blocksSize = helper ? helper->sizeHint(block, index, maxWidth, option) : QSize(0, 0);
                    layout.blocksRectList.append(QRect(layout.senderRect.x(), topBlock, blocksSize.width(), blocksSize.height()));
                    topBlock += blocksSize.height();
                } else {
                    const QSize blockSize = helper ? helper->sizeHint(block, index, maxWidth, option) : QSize(0, 0);
                    layout.blocksRectList.append(QRect(layout.senderRect.x(), topBlock, blockSize.width(), blockSize.height()));
                    blocksSize = QSize(qMax(blocksSize.width(), blockSize.width()), blockSize.height() + blocksSize.height());
                    topBlock += blockSize.height();
                }
            }
            // qDebug() << " topBlock " << topBlock;
            layout.blocksRect = QRect(textLeft, topBlock, blocksSize.width(), blocksSize.height());
        }
        layout.reactionsY = attachmentsY + layout.attachmentsRect.height() + layout.blocksRect.height();
    }
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
