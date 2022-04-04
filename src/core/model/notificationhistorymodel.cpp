/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorymodel.h"

NotificationHistoryModel::NotificationHistoryModel(QObject *parent)
    : QAbstractItemModel{parent}
{
}

NotificationHistoryModel::~NotificationHistoryModel()
{
}

int NotificationHistoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mNotificationInfo.count();
}

QVariant NotificationHistoryModel::data(const QModelIndex &index, int role) const
{
    //    if (index.row() < 0 || index.row() >= mSwichChannelsInfo.count()) {
    //        return {};
    //    }
    //    const SwitchChannelInfo info = mSwichChannelsInfo.at(index.row());
    //    switch (role) {
    //    case Qt::DisplayRole:
    //        return info.mName;
    //    }
    return {};
}

void NotificationHistoryModel::clear()
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mNotificationInfo.count() - 1);
        mNotificationInfo.clear();
        endRemoveRows();
    }
}

void NotificationHistoryModel::insertNotifications(const QVector<NotificationInfo> &infos)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mNotificationInfo.count() - 1);
        mNotificationInfo.clear();
        endRemoveRows();
    }
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mNotificationInfo = infos;
        endInsertRows();
    }
}
