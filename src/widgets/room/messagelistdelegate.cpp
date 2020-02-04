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
#include "messagedelegatehelperreactions.h"
#include "model/messagemodel.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "rocketchataccount.h"

#include <QAbstractItemView>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmapCache>

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mEditedIcon(QIcon::fromTheme(QStringLiteral("document-edit")))
    , mHelperText(new MessageDelegateHelperText)
    , mHelperImage(new MessageDelegateHelperImage)
    , mHelperFile(new MessageDelegateHelperFile)
    , mHelperReactions(new MessageDelegateHelperReactions)
{
}

MessageListDelegate::~MessageListDelegate()
{
}

void MessageListDelegate::setRocketChatAccount(RocketChatAccount *rcAccount)
{
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

static void drawTimestamp(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, const QRect &usableRect)
{
    const qreal margin = basicMargin();

    const QString timeStampText = makeTimeStampText(index);
    const QSize timeSize = timeStampSize(timeStampText, option);

    const QRect timeRect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignRight | Qt::AlignVCenter, timeSize, usableRect.adjusted(0, 0, -margin/2, 0));
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128); // TimestampText.qml had opacity: .5
    painter->setPen(col);
    painter->drawText(timeRect, timeStampText);
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
// [margin] <pixmap> [margin] <sender> [margin] <editicon> [margin] <text message> [margin] <timestamp>
//                                                                  <attachments>
//                                                                  <reactions>
MessageListDelegate::Layout MessageListDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);

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
    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    const qreal margin = basicMargin();
    const int senderX = option.rect.x() + layout.avatarPixmap.width() + 2 * margin;
    int textLeft = senderX + senderTextSize.width() + margin;

    // Edit icon
    if (message->wasEdited()) {
        textLeft += iconSize + margin;
    }

    // Timestamp
    layout.timeStampText = makeTimeStampText(index);
    layout.timeSize = timeStampSize(layout.timeStampText, option);

    // Message (using the rest of the available width)
    const int widthAfterMessage = layout.timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);
    layout.baseLine = 0;
    const QSize textSize = mHelperText->sizeHint(index, maxWidth, option, &layout.baseLine); // TODO share the QTextDocument
    int attachmentsY;
    const int textVMargin = 3; // adjust this for "compactness"
    if (textSize.isValid()) {
        layout.textRect = QRect(textLeft, usableRect.top() + textVMargin,
                                maxWidth, textSize.height() + textVMargin);
        attachmentsY = layout.textRect.y() + layout.textRect.height();
        layout.baseLine += layout.textRect.top(); // make it absolute
    } else {
        attachmentsY = usableRect.top() + textVMargin;
        layout.baseLine = attachmentsY + option.fontMetrics.ascent();
    }
    layout.usableRect.setLeft(textLeft);

    // Align top of sender rect so it matches the baseline of the richtext
    layout.senderRect = QRectF(senderX, layout.baseLine - senderAscent,
                               senderTextSize.width(), senderTextSize.height());
    // Align top of avatar with top of sender rect
    layout.avatarPos = QPointF(option.rect.x() + margin, layout.senderRect.y());
    // Same for the edit icon
    layout.editedIconRect = QRect(textLeft - iconSize - margin, layout.senderRect.y(), iconSize, iconSize);

    if (!message->attachements().isEmpty()) {
        const MessageDelegateHelperBase *helper = attachmentsHelper(message);
        const QSize attachmentsSize = helper ? helper->sizeHint(index, maxWidth, option) : QSize(0, 0);
        layout.attachmentsRect = QRect(textLeft, attachmentsY, attachmentsSize.width(), attachmentsSize.height());
        layout.reactionsY = attachmentsY + attachmentsSize.height();
    } else {
        layout.reactionsY = attachmentsY;
    }
    layout.reactionsHeight = mHelperReactions->sizeHint(index, maxWidth, option).height();

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

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const Layout layout = doLayout(option, index);

    // Timestamp
    drawTimestamp(painter, index, option, layout.usableRect);

    // Message
    if (layout.textRect.isValid()) {
        const QRect messageRect = layout.textRect;
        mHelperText->draw(painter, messageRect, index, option);
    }

    // Draw the pixmap
    painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);

    // Draw the sender
    painter->setFont(layout.senderFont);
    painter->drawText(layout.senderRect.x(), layout.baseLine, layout.senderText);

    // Draw the edited icon
    if (message->wasEdited()) {
        mEditedIcon.paint(painter, layout.editedIconRect);
    }

    // Attachments
    const MessageDelegateHelperBase *helper = attachmentsHelper(message);
    if (helper) {
        helper->draw(painter, layout.attachmentsRect, index, option);
    }

    // Reactions
    const QRect reactionsRect(layout.usableRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
    mHelperReactions->draw(painter, reactionsRect, index, option);

    //drawFocus(painter, option, messageRect);

    // debug painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    painter->restore();
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }

    // contents is date + text + attachments + reactions (where all of those are optional)
    const int contentsHeight = layout.reactionsY + layout.reactionsHeight - option.rect.y();
    const int senderAndAvatarHeight = qMax<int>(layout.senderRect.y() + layout.senderRect.height() - option.rect.y(),
                                                layout.avatarPos.y() + layout.avatarPixmap.height() - option.rect.y());

    //qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height()
    //         << "attachments" << layout.attachmentsRect.height() << "reactions" << layout.reactionsHeight << "total contents" << contentsHeight;
    //qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    return QSize(option.rect.width(),
                 qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight);
}

bool MessageListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        QMouseEvent *mev = static_cast<QMouseEvent *>(event);
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

        const Layout layout = doLayout(option, index);
        if (!message->reactions().isEmpty()) {
            const QRect reactionsRect(layout.usableRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
            if (mHelperReactions->handleMouseEvent(mev, reactionsRect, option, message)) {
                return true;
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

bool MessageListDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

        if (!message->reactions().isEmpty()) {
            const Layout layout = doLayout(option, index);
            const QRect reactionsRect(layout.usableRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
            if (mHelperReactions->handleHelpEvent(helpEvent, view, reactionsRect, option, message)) {
                return true;
            }
        }
    }
    return false;
}
