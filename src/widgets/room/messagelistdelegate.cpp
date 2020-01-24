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
#include "model/messagemodel.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticon.h"

#include <QPainter>
#include <QPixmapCache>
#include <QStyle>
#include <QTextBlock>
#include <QTextDocument>
#include <QWindow>


MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent), m_emojiFont(QStringLiteral("NotoColorEmoji"))
{
}

static void drawRichText(QPainter *painter, const QRect &rect, const QString &text, qreal *pBaseLine)
{
    // Possible optimisation: store the QTextDocument into the Message itself?
    QTextDocument doc;
    doc.setHtml(text);
    doc.setTextWidth(rect.width());

    //QStyleOptionViewItemV4 options = option;
    //initStyleOption(&options, index);
    //options.text = QString();
    //options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    painter->translate(rect.left(), rect.top());
    const QRect clip(0, 0, rect.width(), rect.height());
    doc.drawContents(painter, clip);
    painter->translate(-rect.left(), -rect.top());

    const qreal frameMargin = doc.frameAt(0)->frameFormat().topMargin();
    const QTextLine &line = doc.firstBlock().layout()->lineAt(0);
    *pBaseLine = rect.y() + frameMargin + line.y() + line.ascent();
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

static void drawTimestamp(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, QRect *timeRect)
{
    const QString timeStampText = makeTimeStampText(index);
    const QSize timeSize = timeStampSize(timeStampText, option);

    *timeRect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignRight | Qt::AlignVCenter, timeSize, option.rect);
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128); // TimestampText.qml had opacity: .5
    painter->setPen(col);
    painter->drawText(*timeRect, timeStampText);
    painter->setPen(oldPen);

    painter->setPen(oldPen);
}

static QPixmap makeAvatarPixmap(const QModelIndex &index, int maxHeight)
{
    const QString userId = index.data(MessageModel::UserId).toString();
    const QString iconUrlStr = Ruqola::self()->rocketChatAccount()->avatarUrl(userId);
    QPixmap pix;
    if (!iconUrlStr.isEmpty() && !QPixmapCache::find(iconUrlStr, &pix)) {
        const QUrl iconUrl(iconUrlStr);
        Q_ASSERT(iconUrl.isLocalFile());
        if (pix.load(iconUrl.toLocalFile())) {
            pix = pix.scaledToHeight(maxHeight);
            QPixmapCache::insert(iconUrlStr, pix);
        } else {
            qWarning() << "Could not load" << iconUrl.toLocalFile();
        }
    }
    return pix;
}

static qreal basicMargin()
{
    return 8;
}

struct PixmapAndSenderLayout {
    QString senderText;
    QFont senderFont;
    QRectF senderRect;
    qreal ascent;
    QPixmap avatarPixmap;
    qreal avatarX;
};

// [margin] <pixmap> [margin] <sender>
static PixmapAndSenderLayout layoutPixmapAndSender(const QStyleOptionViewItem &option,
                                                   const QModelIndex &index)
{
    PixmapAndSenderLayout layout;
    layout.senderText = makeSenderText(index);
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const QSizeF senderTextSize = senderFontMetrics.boundingRect(layout.senderText).size();

    layout.avatarPixmap = makeAvatarPixmap(index, senderTextSize.height());
    layout.ascent = senderFontMetrics.ascent();

    const qreal margin = basicMargin();
    layout.senderRect = QRectF(option.rect.x() + layout.avatarPixmap.width() + 2 * margin, option.rect.y(),
                               senderTextSize.width(), senderTextSize.height());
    layout.avatarX = option.rect.x() + margin;
    return layout;
}

QString MessageListDelegate::makeMessageText(const QModelIndex &index) const
{
    return index.data(MessageModel::MessageConvertedText).toString();
}

QVector<MessageListDelegate::ReactionLayout> MessageListDelegate::layoutReactions(const QVariantList &reactions, const qreal messageX, const QStyleOptionViewItem &option) const
{
    QVector<MessageListDelegate::ReactionLayout> layout;
    layout.reserve(reactions.count());
    auto *emojiManager = Ruqola::self()->rocketChatAccount()->emojiManager();
    QFontMetricsF emojiFontMetrics(m_emojiFont);
    const qreal margin = basicMargin();
    const qreal smallMargin = margin/2.0;
    qreal x = messageX + margin;

    for (const QVariant &v : reactions) {
        const Reaction &reaction = v.value<Reaction>();
        const QString emojiString = emojiManager->unicodeEmoticonForEmoji(reaction.reactionName()).unicode();
        if (!emojiString.isEmpty()) {
            const QSizeF emojiSize = emojiFontMetrics.boundingRect(emojiString).size();
            const qreal y = option.rect.bottom() - emojiFontMetrics.height() - smallMargin;
            const QString countStr = QString::number(reaction.count());
            const int countWidth = option.fontMetrics.horizontalAdvance(countStr) + smallMargin;
            const QRectF reactionRect(x, y, emojiSize.width() + countWidth + margin,
                                      qMax<qreal>(emojiSize.height(), option.fontMetrics.height()) + margin - 1);
            const qreal emojiOffset = margin / 2 + 1;
            const QRectF countRect = reactionRect.adjusted(emojiOffset + emojiSize.width(), smallMargin, 0, 0);

            layout.append(ReactionLayout{reactionRect, countRect, emojiString, countStr, emojiOffset, reaction});
            x += reactionRect.width() + margin;
        } else {
            // TODO other kinds of emojis (but how to handle an image URL? QTextDocument manages somehow, I don't get it)
            static QString lastWarning;
            if (lastWarning != reaction.reactionName()) {
                lastWarning = reaction.reactionName();
                qDebug() << "Not handled: emoji" << reaction.reactionName() << emojiManager->replaceEmojiIdentifier(reaction.reactionName(), true);
            }
        }
    }
    return layout;
}

void MessageListDelegate::drawReactions(QPainter *painter, const QModelIndex &index, const QRect &messageRect, const QStyleOptionViewItem &option) const
{
    const QVariantList reactions = index.data(MessageModel::Reactions).toList();
    if (reactions.isEmpty()) {
        return;
    }

    const QVector<ReactionLayout> layout = layoutReactions(reactions, messageRect.x(), option);

    const QPen origPen = painter->pen();
    const QBrush origBrush = painter->brush();
    const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
    QColor backgroundColor = option.palette.color(QPalette::Highlight);
    backgroundColor.setAlpha(60);
    const QBrush buttonBrush(backgroundColor);
    const qreal smallMargin = basicMargin()/2.0;
    for (const ReactionLayout &reactionLayout : layout) {
        // ### Optimization idea: MessageModel::Message role, and calling the Message API directly
        // Especially interesting in sizeHint where we just need to know "there are reactions"

        if (!reactionLayout.emojiString.isEmpty()) {

            const QRectF reactionRect = reactionLayout.reactionRect;

            // Rounded rect
            painter->setFont(m_emojiFont);
            painter->setPen(buttonPen);
            painter->setBrush(buttonBrush);
            painter->drawRoundedRect(reactionRect, 5, 5);
            painter->setBrush(origBrush);
            painter->setPen(origPen);

            // Emoji
            painter->drawText(reactionRect.adjusted(reactionLayout.emojiOffset, smallMargin, 0, 0), reactionLayout.emojiString);

            // Count
            painter->setFont(option.font);
            painter->drawText(reactionLayout.countRect, reactionLayout.countStr);

        }
    }
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);

    const QPen origPen = painter->pen();

    // Compact mode : <pixmap> <sender> <message> <smiley> <timestamp>

    /*auto lst = index.data(MessageModel::Attachments).toList();
    if (!lst.isEmpty()) {
        qDebug() << "Attachments" << lst;
    }
    auto urls = index.data(MessageModel::Urls).toList();
    if (!urls.isEmpty()) {
        qDebug() << "Urls" << urls;
    }*/

    // Sender and pixmap (calculate size, but don't draw it yet, we need to align vertically to the first line of the message)
    const PixmapAndSenderLayout leftLayout = layoutPixmapAndSender(option, index);

    // Timestamp
    QRect timeRect;
    drawTimestamp(painter, index, option, &timeRect);

    // Message
    QRect messageRect = option.rect;
    messageRect.setLeft(leftLayout.senderRect.right());
    messageRect.setRight(timeRect.left() - 1);
    const QString message = makeMessageText(index);
    qreal baseLine = 0;
    drawRichText(painter, messageRect, message, &baseLine);

    // Now draw the pixmap
    painter->drawPixmap(leftLayout.avatarX, baseLine - leftLayout.ascent, leftLayout.avatarPixmap);
    // If we need support for drawing as selected, we might want to do this:
    //QRect decorationRect(option.rect.x(), topOfFirstLine, avatarPixmap.width(), avatarPixmap.height());
    //drawDecoration(painter, option, decorationRect, avatarPixmap);

    // Now draw the sender
    painter->setFont(leftLayout.senderFont);
    painter->drawText(leftLayout.senderRect.x(), baseLine, leftLayout.senderText);

    // Reactions
    drawReactions(painter, index, messageRect, option);

    painter->setFont(option.font);
    painter->setPen(origPen);

    //drawFocus(painter, option, displayRect);
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Avatar pixmap and sender text
    const PixmapAndSenderLayout leftLayout = layoutPixmapAndSender(option, index);

    // Timestamp
    const QString timeStampText = makeTimeStampText(index);
    const QSize timeSize = timeStampSize(timeStampText, option);

    // Message (using the rest of the available width)
    QTextDocument doc;
    const QString message = makeMessageText(index);
    doc.setHtml(message);
    const int widthBeforeMessage = leftLayout.senderRect.right();
    const int widthAfterMessage = timeSize.width();
    doc.setTextWidth(qMax(30, option.rect.width() - widthBeforeMessage - widthAfterMessage));

    const qreal margin = basicMargin();

    int additionalHeight = 0;
    const QVariantList reactions = index.data(MessageModel::Reactions).toList();
    if (!reactions.isEmpty()) {
        QFontMetricsF emojiFontMetrics(m_emojiFont);
        additionalHeight += emojiFontMetrics.height() + margin;
    }

    // hopefully the width below is never more than option.rect.width() or we'll get a scrollbar
    return QSize(widthBeforeMessage + doc.idealWidth() + widthAfterMessage,
                 qMax<int>(leftLayout.senderRect.height(), doc.size().height()) + additionalHeight);
}

bool MessageListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mev = static_cast<QMouseEvent *>(event);
        const QPoint pos = mev->pos();
        const QVariantList reactions = index.data(MessageModel::Reactions).toList();
        if (!reactions.isEmpty()) {
            auto *rcAccount = Ruqola::self()->rocketChatAccount();
            const PixmapAndSenderLayout leftLayout = layoutPixmapAndSender(option, index);
            const QVector<ReactionLayout> layout = layoutReactions(reactions, leftLayout.senderRect.right(), option);
            for (const ReactionLayout &reactionLayout : layout) {
                if (reactionLayout.reactionRect.contains(pos)) {
                    const Reaction &reaction = reactionLayout.reaction;
                    const bool doAdd = !reaction.userNames().contains(rcAccount->userName());
                    rcAccount->reactOnMessage(index.data(MessageModel::MessageId).toString(), reaction.reactionName(), doAdd);
                    break;
                }
            }
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}
