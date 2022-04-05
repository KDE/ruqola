/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydelegate.h"

#include <QPainter>

NotificationHistoryDelegate::NotificationHistoryDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

NotificationHistoryDelegate::~NotificationHistoryDelegate()
{
}

void NotificationHistoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    // TODO
    painter->restore();
}
