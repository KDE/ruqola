/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistorymodel.h"
#include <KLocalizedString>

ServerErrorInfoHistoryModel::ServerErrorInfoHistoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ServerErrorInfoHistoryModel::~ServerErrorInfoHistoryModel() = default;

int ServerErrorInfoHistoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mServerErrorInfo.count();
}

QVariant ServerErrorInfoHistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mServerErrorInfo.count()) {
        return {};
    }
    const auto info = mServerErrorInfo.at(index.row());
#if 0
    switch (role) {
    case Qt::DisplayRole:
    case AccountName:
        return info.accountName();
    case DateTime:
        return info.dateTime();
    case MessageStr:
        return generateMessage(info);
    case RoomId:
        return info.roomId();
    case ChannelType:
        return info.channelType();
    case Pixmap:
        return info.pixmap();
    case SenderName:
        return info.senderName();
    case SenderUserName:
        return info.senderUserName();
    case MessageId:
        return info.messageId();
    case RoomName:
        return info.roomName();
    }
#endif
    return {};
}

void ServerErrorInfoHistoryModel::clear()
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mServerErrorInfo.count() - 1);
        mServerErrorInfo.clear();
        endRemoveRows();
    }
}
#if 0
void ServerErrorInfoHistoryModel::insertNotifications(const QVector<NotificationInfo> &infos)
{
#if 0
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
#endif
}

void ServerErrorInfoHistoryModel::addNotification(const NotificationInfo &info)
{
    const int numberOfElement = mNotificationInfo.count();
    mNotificationInfo.append(info);
    beginInsertRows(QModelIndex(), numberOfElement, mNotificationInfo.count() - 1);
    endInsertRows();
}
#endif
