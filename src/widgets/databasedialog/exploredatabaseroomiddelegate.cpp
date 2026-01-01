/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabaseroomiddelegate.h"
#include "rocketchataccount.h"
#include <QHelpEvent>
#include <QToolTip>

ExploreDatabaseRoomIdDelegate::ExploreDatabaseRoomIdDelegate(RocketChatAccount *account, QObject *parent)
    : QStyledItemDelegate{parent}
    , mRocketChatAccount(account)
{
}

ExploreDatabaseRoomIdDelegate::~ExploreDatabaseRoomIdDelegate() = default;

QString ExploreDatabaseRoomIdDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (mRocketChatAccount) {
        if (auto r = mRocketChatAccount->room(value.toByteArray()); r) {
            return r->displayRoomName();
        }
    }
    return QStyledItemDelegate::displayText(value, locale);
}

bool ExploreDatabaseRoomIdDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!event || !view)
        return false;

    if (event->type() == QEvent::ToolTip) {
        const QString tooltip = index.data(Qt::DisplayRole).toString();
        QToolTip::showText(event->globalPos(), tooltip);
        return true;
    }
    return QStyledItemDelegate::helpEvent(event, view, option, index);
}

#include "moc_exploredatabaseroomiddelegate.cpp"
