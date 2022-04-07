/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydelegate.h"
#include "model/notificationhistorymodel.h"
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

    // TODO
    painter->restore();
}

NotificationHistoryDelegate::Layout NotificationHistoryDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NotificationHistoryDelegate::Layout layout;
    const QString userName = index.data(NotificationHistoryModel::SenderName).toString();
    layout.senderText = QLatin1Char('@') + userName;
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    // TODO
    return layout;
}
