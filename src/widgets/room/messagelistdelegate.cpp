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

#include "messagelistdelegate.h"
#include "messagedelegatehelperbase.h"
#include "messagedelegatehelpertext.h"
#include "messagedelegatehelperimage.h"
#include "messagedelegatehelperfile.h"
#include "model/messagemodel.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "rocketchataccount.h"
#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticon.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPixmapCache>

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mEmojiFont(QStringLiteral("NotoColorEmoji"))
    , mHelperText(new MessageDelegateHelperText)
    , mHelperImage(new MessageDelegateHelperImage)
    , mHelperFile(new MessageDelegateHelperFile)
{
}

MessageListDelegate::~MessageListDelegate()
{
}

void MessageListDelegate::setRocketChatAccount(RocketChatAccount *rcAccount)
{
//    if (mRocketChatAccount) {
//        disconnect(mRocketChatAccount, 0, this, 0);
//    }
    mRocketChatAccount = rcAccount;
}

static qreal basicMargin()
{
    return 8;
}

static QString makeSenderText(const QModelIndex &index)
{
    return QLatin1Char('@') + index.data(MessageModel::Username).toString();
}

static QString makeTimeStampText(const QModelIndex &index)
{
    return index.data(MessageModel::Timestamp).toString();
}

static QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option)
{
    // This gives incorrect results (too small bounding rect), no idea why!
    //const QSize timeSize = painter->fontMetrics().boundingRect(timeStampText).size();
    return QSize(option.fontMetrics.horizontalAdvance(timeStampText), option.fontMetrics.height());
}

static void drawTimestamp(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, const QRect &usableRect, QRect *timeRect)
{
    const qreal margin = basicMargin();

    const QString timeStampText = makeTimeStampText(index);
    const QSize timeSize = timeStampSize(timeStampText, option);

    *timeRect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignRight | Qt::AlignVCenter, timeSize, usableRect.adjusted(0, 0, -margin/2, 0));
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128); // TimestampText.qml had opacity: .5
    painter->setPen(col);
    painter->drawText(*timeRect, timeStampText);
    painter->setPen(oldPen);
}

QPixmap MessageListDelegate::makeAvatarPixmap(const QModelIndex &index, int maxHeight) const
{
    const QString userId = index.data(MessageModel::UserId).toString();
    const QString iconUrlStr = mRocketChatAccount->avatarUrl(userId);
    QPixmap pix;
    if (!iconUrlStr.isEmpty() && !QPixmapCache::find(iconUrlStr, &pix)) {
        const QUrl iconUrl(iconUrlStr);
        Q_ASSERT(iconUrl.isLocalFile());
        if (pix.load(iconUrl.toLocalFile())) {
            pix = pix.scaledToHeight(maxHeight);
            QPixmapCache::insert(iconUrlStr, pix);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << iconUrl.toLocalFile();
        }
    }
    return pix;
}

// [Optional date header]
// [margin] <pixmap> [margin] <sender> [margin] <text message> [margin] <timestamp>
//                                              <attachments>
//                                              <reactions>
MessageListDelegate::Layout MessageListDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Layout layout;
    layout.senderText = makeSenderText(index);
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);

    layout.avatarPixmap = makeAvatarPixmap(index, senderTextSize.height());

    QRect usableRect = option.rect;
    if (index.data(MessageModel::DateDiffersFromPrevious).toBool()) {
        usableRect.setTop(usableRect.top() + option.fontMetrics.height());
    }
    layout.usableRect = usableRect;

    const qreal margin = basicMargin();
    layout.senderRect = QRectF(option.rect.x() + layout.avatarPixmap.width() + 2 * margin, usableRect.y(),
                               senderTextSize.width(), senderTextSize.height());

    // Timestamp
    layout.timeStampText = makeTimeStampText(index);
    layout.timeSize = timeStampSize(layout.timeStampText, option);

    // Message (using the rest of the available width)
    const int widthBeforeMessage = layout.senderRect.right() + margin;
    const int widthAfterMessage = layout.timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - widthBeforeMessage - widthAfterMessage);
    layout.baseLine = 0;
    const QSize textSize = mHelperText->sizeHint(index, maxWidth, option, &layout.baseLine); // TODO share the QTextDocument
    const int textLeft = layout.senderRect.right() + margin;
    int attachmentsY;
    if (textSize.isValid()) {
        layout.textRect = QRect(textLeft, usableRect.top() + margin,
                                maxWidth, textSize.height() + margin);
        attachmentsY = layout.textRect.bottom();
        layout.baseLine += layout.textRect.top(); // make it absolute
    } else {
        attachmentsY = usableRect.top() + margin;
        layout.baseLine = attachmentsY + option.fontMetrics.ascent();
    }
    layout.usableRect.setLeft(textLeft);

    layout.avatarPos = QPointF(option.rect.x() + margin,
                               layout.baseLine - senderAscent);

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const QVector<Reaction> reactions = message->reactions().reactions();
    if (!reactions.isEmpty()) {
        QFontMetricsF emojiFontMetrics(mEmojiFont);
        layout.reactionsHeight = qMax<qreal>(emojiFontMetrics.height(), option.fontMetrics.height()) + 7 + margin;
    } else {
        layout.reactionsHeight = 0;
    }

    if (!message->attachements().isEmpty()) {
        layout.attachmentsRect = QRect(textLeft, attachmentsY,
                                       maxWidth, usableRect.height() - (attachmentsY - usableRect.top()) - layout.reactionsHeight);
    }

    return layout;
}

MessageDelegateHelperBase *MessageListDelegate::attachmentsHelper(const Message *message) const
{
    switch (message->messageType()) {
    case Message::Image:
        return mHelperImage.data();
    case Message::File:
        return mHelperFile.data();
    default:
        break;
    }
    return nullptr;
}

QVector<MessageListDelegate::ReactionLayout> MessageListDelegate::layoutReactions(const QVector<Reaction> &reactions, const qreal messageX, const QStyleOptionViewItem &option) const
{
    QVector<MessageListDelegate::ReactionLayout> layouts;
    layouts.reserve(reactions.count());
    auto *emojiManager = mRocketChatAccount->emojiManager();
    QFontMetricsF emojiFontMetrics(mEmojiFont);
    const qreal margin = basicMargin();
    const qreal smallMargin = margin/2.0;
    const qreal height = qMax<qreal>(emojiFontMetrics.height(), option.fontMetrics.height()) + 7; // also stored in Layout
    qreal x = messageX + margin;

    for (const Reaction &reaction : reactions) {
        ReactionLayout layout;
        layout.emojiString = emojiManager->unicodeEmoticonForEmoji(reaction.reactionName()).unicode();
        qreal emojiWidth;
        if (layout.emojiString.isEmpty()) {
            layout.emojiString = reaction.reactionName(); // ugly fallback: ":1md"
            emojiWidth = option.fontMetrics.horizontalAdvance(layout.emojiString) + smallMargin;
            layout.useEmojiFont = false;
        } else {
            emojiWidth = emojiFontMetrics.horizontalAdvance(layout.emojiString);
            layout.useEmojiFont = true;
        }
        layout.countStr = QString::number(reaction.count());
        const int countWidth = option.fontMetrics.horizontalAdvance(layout.countStr) + smallMargin;
        layout.reactionRect = QRectF(x, option.rect.bottom() - height, emojiWidth + countWidth + margin, height);
        layout.emojiOffset = margin / 2 + 1;
        layout.countRect = layout.reactionRect.adjusted(layout.emojiOffset + emojiWidth, smallMargin, 0, 0);
        layout.reaction = reaction;

        layouts.append(layout);
        x += layout.reactionRect.width() + margin;
    }
    return layouts;
}

void MessageListDelegate::drawReactions(QPainter *painter, const QModelIndex &index, const QRect &usableRect, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const QVector<Reaction> reactions = message->reactions().reactions();
    if (reactions.isEmpty()) {
        return;
    }

    const QVector<ReactionLayout> layout = layoutReactions(reactions, usableRect.x(), option);

    const QPen origPen = painter->pen();
    const QBrush origBrush = painter->brush();
    const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
    QColor backgroundColor = option.palette.color(QPalette::Highlight);
    backgroundColor.setAlpha(60);
    const QBrush buttonBrush(backgroundColor);
    const qreal smallMargin = basicMargin()/2.0;
    for (const ReactionLayout &reactionLayout : layout) {
        if (!reactionLayout.emojiString.isEmpty()) {
            const QRectF reactionRect = reactionLayout.reactionRect;

            // Rounded rect
            painter->setPen(buttonPen);
            painter->setBrush(buttonBrush);
            painter->drawRoundedRect(reactionRect, 5, 5);
            painter->setBrush(origBrush);
            painter->setPen(origPen);

            // Emoji
            if (reactionLayout.useEmojiFont) {
                painter->setFont(mEmojiFont);
            }
            painter->drawText(reactionRect.adjusted(reactionLayout.emojiOffset, smallMargin, 0, 0), reactionLayout.emojiString);

            // Count
            painter->setFont(option.font);
            painter->drawText(reactionLayout.countRect, reactionLayout.countStr);
        }
    }
}

void MessageListDelegate::drawDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const QPen origPen = painter->pen();
    const qreal margin = basicMargin();
    const QString dateStr = index.data(MessageModel::Date).toString();
    const QSize dateSize = option.fontMetrics.size(Qt::TextSingleLine, dateStr);
    const QRect dateAreaRect(option.rect.x(), option.rect.y(), option.rect.width(), dateSize.height()); // the whole row
    const QRect dateTextRect = QStyle::alignedRect(Qt::LayoutDirectionAuto, Qt::AlignCenter, dateSize, dateAreaRect);
    painter->drawText(dateTextRect, dateStr);
    const int lineY = (dateAreaRect.top() + dateAreaRect.bottom()) / 2;
    QColor lightColor(painter->pen().color());
    lightColor.setAlpha(60);
    painter->setPen(lightColor);
    painter->drawLine(dateAreaRect.left(), lineY, dateTextRect.left() - margin, lineY);
    painter->drawLine(dateTextRect.right() + margin, lineY, dateAreaRect.right(), lineY);
    painter->setPen(origPen);
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    drawBackground(painter, option, index);

    // Draw date if it differs from the previous message
    if (index.data(MessageModel::DateDiffersFromPrevious).toBool()) {
        drawDate(painter, index, option);
    }

    // Compact mode : <pixmap> <sender> <message> <smiley> <timestamp>

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    // Sender and pixmap (calculate size, but don't draw it yet, we need to align vertically to the first line of the message)
    const Layout layout = doLayout(option, index);

    // Timestamp
    QRect timeRect; // REMOVE
    drawTimestamp(painter, index, option, layout.usableRect, &timeRect);

    // Message
    if (layout.textRect.isValid()) {
        const QRect messageRect = layout.textRect;
        mHelperText->draw(painter, messageRect, index, option);
    }

    // Now draw the pixmap
    painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);

    // Now draw the sender
    painter->setFont(layout.senderFont);
    painter->drawText(layout.senderRect.x(), layout.baseLine, layout.senderText);

    // Attachments
    const MessageDelegateHelperBase *helper = attachmentsHelper(message);
    if (helper) {
        helper->draw(painter, layout.attachmentsRect, index, option);
    }

    // Reactions
    drawReactions(painter, index, layout.usableRect, option);

    //drawFocus(painter, option, messageRect);

    // debug painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    painter->restore();
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const Layout layout = doLayout(option, index);

    const MessageDelegateHelperBase *helper = attachmentsHelper(message);
    const QSize attachmentsSize = helper ? helper->sizeHint(index, layout.textRect.width(), option) : QSize(0, 0);

    int additionalHeight = layout.reactionsHeight;
    if (index.data(MessageModel::DateDiffersFromPrevious).toBool()) {
        additionalHeight += option.fontMetrics.height();
    }

    //const QSize size(qMax(layout.textRect.width(), attachmentsSize.width()), layout.textRect.height() + attachmentsSize.height());
    const int textAndAttachHeight = layout.textRect.height() + attachmentsSize.height();
    const int senderAndAvatarHeight = qMax<int>(layout.senderRect.height(),
                                                layout.avatarPos.y() + layout.avatarPixmap.height() - layout.usableRect.y());

    //qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height() << "attachments" << attachmentsSize.height() << "reactions" << layout.reactionsHeight << "total additional" << additionalHeight;
    //qDebug() << "=> returning" << qMax(senderAndAvatarHeight, textAndAttachHeight) + additionalHeight + 1;

    return QSize(option.rect.width(),
                 qMax(senderAndAvatarHeight, textAndAttachHeight) + additionalHeight + 1);
}

bool MessageListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        QMouseEvent *mev = static_cast<QMouseEvent *>(event);
        const QPoint pos = mev->pos();
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

        const Layout layout = doLayout(option, index);
        if (!message->reactions().isEmpty()) {
            const QVector<ReactionLayout> reactions = layoutReactions(message->reactions().reactions(), layout.senderRect.right(), option);
            for (const ReactionLayout &reactionLayout : reactions) {
                if (reactionLayout.reactionRect.contains(pos)) {
                    const Reaction &reaction = reactionLayout.reaction;
                    const bool doAdd = !reaction.userNames().contains(mRocketChatAccount->userName());
                    mRocketChatAccount->reactOnMessage(index.data(MessageModel::MessageId).toString(), reaction.reactionName(), doAdd);
                    return true;
                }
            }
        }
        if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
            return true;
        }

        MessageDelegateHelperBase *helper = attachmentsHelper(message);
        if (helper && helper->handleMouseEvent(mev, layout.attachmentsRect, option, index)) {
            return true;
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}
