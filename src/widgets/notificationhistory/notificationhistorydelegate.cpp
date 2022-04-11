/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydelegate.h"
#include "common/delegatepaintutil.h"
#include "model/notificationhistorymodel.h"
#include "room/delegate/messagedelegateutils.h"
#include <QPainter>

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
        // mHelperText->draw(painter, layout.textRect, index, option);
    }

    // Draw the pixmap
    painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);
    // TODO
    painter->restore();
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

    // Timestamp
    layout.timeStampText = index.data(NotificationHistoryModel::DateTime).toString();
    const QSize timeSize = timeStampSize(layout.timeStampText, option);
    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);

    // Message (using the rest of the available width)
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);
    const int senderX = option.rect.x() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).width() + 2 * margin;
    int textLeft = senderX + senderTextSize.width() + margin;
    const int widthAfterMessage = iconSize + margin + timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);

    // TODO
    return layout;
}
