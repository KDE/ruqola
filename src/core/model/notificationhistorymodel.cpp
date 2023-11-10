/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorymodel.h"
#include <KLocalizedString>

NotificationHistoryModel::NotificationHistoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

NotificationHistoryModel::~NotificationHistoryModel() = default;

int NotificationHistoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mNotificationInfo.count();
}

QVariant NotificationHistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mNotificationInfo.count()) {
        return {};
    }
    const auto info = mNotificationInfo.at(index.row());
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
    return {};
}

QString NotificationHistoryModel::generateMessage(const NotificationInfo &info) const
{
    switch (info.notificationType()) {
    case NotificationInfo::StandardMessage:
        return info.message();
    case NotificationInfo::ConferenceCall:
        return i18n("Conference Call");
    }
    return {};
}

void NotificationHistoryModel::clear()
{
    if (!mNotificationInfo.isEmpty()) {
        beginResetModel();
        mNotificationInfo.clear();
        endResetModel();
    }
}

void NotificationHistoryModel::insertNotifications(const QVector<NotificationInfo> &infos)
{
    clear();
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mNotificationInfo = infos;
        endInsertRows();
    }
}

void NotificationHistoryModel::addNotification(const NotificationInfo &info)
{
    if (mNotificationInfo.contains(info)) {
        return;
    }
    const int numberOfElement = mNotificationInfo.count();
    mNotificationInfo.append(info);
    beginInsertRows(QModelIndex(), numberOfElement, mNotificationInfo.count() - 1);
    endInsertRows();
}

#include "moc_notificationhistorymodel.cpp"
