/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydelegate.h"
#include "common/delegatepaintutil.h"
#include "model/notificationhistorymodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messagedelegateutils.h"
#include "ruqola.h"
#include "textconverter.h"
#include <QPainter>
#include <QTextBlock>

NotificationHistoryDelegate::NotificationHistoryDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

NotificationHistoryDelegate::~NotificationHistoryDelegate()
{
}

static QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option)
{
    // This gives incorrect results (too small bounding rect), no idea why!
    // const QSize timeSize = painter->fontMetrics().boundingRect(timeStampText).size();
    return {option.fontMetrics.horizontalAdvance(timeStampText), option.fontMetrics.height()};
}

void NotificationHistoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    drawBackground(painter, option, index);

    const Layout layout = doLayout(option, index);

    // Timestamp
    DelegatePaintUtil::drawLighterText(painter, layout.timeStampText, layout.timeStampPos);
    if (layout.textRect.isValid()) {
        auto *doc = documentForIndex(index, layout.textRect.width());
        if (doc) {
            MessageDelegateUtils::drawSelection(doc, layout.textRect, layout.textRect.top(), painter, index, option, nullptr, {});
        }
    }

    // Draw the pixmap
    painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);

    // debug
    painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    painter->restore();
}

void NotificationHistoryDelegate::clearCache()
{
    mDocumentCache.clear();
}

QSize NotificationHistoryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }

    // contents is date + text + attachments + reactions + replies + discussions (where all of those are optional)
    const int contentsHeight = layout.textRect.y() + layout.textRect.height();
    const int senderAndAvatarHeight = qMax<int>(layout.senderRect.y() + layout.senderRect.height() - option.rect.y(),
                                                layout.avatarPos.y() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).height() - option.rect.y());

    //    qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height() << "total contents" << contentsHeight;
    //    qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    return {option.rect.width(), qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight};
}

QSize NotificationHistoryDelegate::textSizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(option)
    auto *doc = documentForIndex(index, maxWidth);
    if (!doc) {
        return {};
    }
    const QSize size(doc->idealWidth(), doc->size().height()); // do the layouting, required by lineAt(0) below

    const QTextLine &line = doc->firstBlock().layout()->lineAt(0);
    *pBaseLine = line.y() + line.ascent(); // relative

    return size;
}
// [margin] <pixmap> [margin] <sender> [margin] <text message> [margin] <add reaction> [margin]
NotificationHistoryDelegate::Layout NotificationHistoryDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NotificationHistoryDelegate::Layout layout;
    const QString userName = index.data(NotificationHistoryModel::SenderName).toString();
    const int margin = MessageDelegateUtils::basicMargin();
    layout.senderText = QLatin1Char('@') + userName;
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);

    layout.avatarPixmap = index.data(NotificationHistoryModel::Pixmap).value<QPixmap>();

    // Timestamp
    layout.timeStampText = index.data(NotificationHistoryModel::DateTime).toString();

    // Message (using the rest of the available width)
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);
    const int senderX = option.rect.x() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).width() + 2 * margin;
    int textLeft = senderX + senderTextSize.width() + margin;
    const QSize timeSize = timeStampSize(layout.timeStampText, option);
    const int widthAfterMessage = iconSize + margin + timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);
    layout.baseLine = 0;
    const QSize textSize = textSizeHint(index, maxWidth, option, &layout.baseLine);

    // Align top of sender rect so it matches the baseline of the richtext
    layout.senderRect = QRectF(senderX, layout.baseLine - senderAscent, senderTextSize.width(), senderTextSize.height());

    const int textVMargin = 3; // adjust this for "compactness"
    QRect usableRect = option.rect;
    layout.textRect = QRect(textLeft, usableRect.top() + textVMargin, maxWidth, textSize.height() + textVMargin);
    layout.baseLine += layout.textRect.top();

    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);

    // Align top of avatar with top of sender rect
    layout.avatarPos = QPointF(option.rect.x() + margin, layout.senderRect.y());

    // TODO
    return layout;
}

QTextDocument *NotificationHistoryDelegate::documentForIndex(const QModelIndex &index, int width) const
{
    Q_ASSERT(index.isValid());
    const QString messageId = index.data(NotificationHistoryModel::MessageId).toString();
    Q_ASSERT(!messageId.isEmpty());

    auto it = mDocumentCache.find(messageId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString messageStr = index.data(NotificationHistoryModel::Message).toString();

    if (messageStr.isEmpty()) {
        return nullptr;
    }
    const QString accountName = index.data(NotificationHistoryModel::AccountName).toString();
    // Use TextConverter in case it starts with a [](URL) reply marker
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QString needUpdateMessageId; // TODO use it ?
    // TODO search rcAccount from account name!
    const QString contextString = TextConverter::convertMessageText(messageStr,
                                                                    rcAccount->userName(),
                                                                    {},
                                                                    rcAccount->highlightWords(),
                                                                    rcAccount->emojiManager(),
                                                                    rcAccount->messageCache(),
                                                                    needUpdateMessageId,
                                                                    {},
                                                                    {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(messageId, std::move(doc));
    return ret;
}
