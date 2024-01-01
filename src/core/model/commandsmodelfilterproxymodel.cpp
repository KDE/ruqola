/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandsmodelfilterproxymodel.h"
#include "commandsmodel.h"
#include "rocketchataccount.h"

CommandsModelFilterProxyModel::CommandsModelFilterProxyModel(RocketChatAccount *account, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mRocketChatAccount(account)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(CommandsModel::CommandName);
    sort(0);
}

void CommandsModelFilterProxyModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

CommandsModelFilterProxyModel::~CommandsModelFilterProxyModel() = default;

bool CommandsModelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, CommandsModel::CommandName).toString();
        const QString rightString = sourceModel()->data(right, CommandsModel::CommandName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

bool CommandsModelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mRocketChatAccount) {
        const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);
        const QStringList permissions = modelIndex.data(CommandsModel::Permissions).toStringList();

        // const QString commandName = modelIndex.data(CommandsModel::CommandName).toString();
        Room *room = mRocketChatAccount->room(mRoomId);
        // qDebug() << " permissions " << permissions << " mRoomId " << mRoomId << " room " << room << " commandName " << commandName;
        for (const QString &permission : permissions) {
            if (permission.isEmpty()) {
                return false;
            }
            // const bool accountHasPermission = mRocketChatAccount->hasPermission(permission);
            // const bool roomHasPermission = room && room->hasPermission(permission);
            // qDebug() << " accountHasPermission " << accountHasPermission << " roomHasPermission " << roomHasPermission << " commandName " << commandName;
            if (!mRocketChatAccount->hasPermission(permission) && !(room && room->hasPermission(permission))) {
                return false;
            }
        }
    }
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

#include "moc_commandsmodelfilterproxymodel.cpp"
