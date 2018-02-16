/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "statusmodel.h"
#include <KLocalizedString>

StatusModel::StatusModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

StatusModel::~StatusModel()
{
}

User::PresenceStatus StatusModel::status(int index) const
{
    return mStatusList.at(index).status;
}

QHash<int, QByteArray> StatusModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[StatusI18n] = QByteArrayLiteral("statusi18n");
    roles[Status] = QByteArrayLiteral("status");
    roles[Icon] = QByteArrayLiteral("icon");
    roles[CurrentStatus] = QByteArrayLiteral("currentstatus");
    return roles;
}

void StatusModel::fillModel()
{
    {
        StatusInfo statusInfo;
        statusInfo.displayText = i18n("Online");
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-online"));
        statusInfo.status = User::PresenceStatus::PresenceOnline;
        mStatusList.append(statusInfo);
    }
    {
        StatusInfo statusInfo;
        statusInfo.displayText = i18n("Busy");
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-busy"));
        statusInfo.status = User::PresenceStatus::PresenceBusy;
        mStatusList.append(statusInfo);
    }
    {
        StatusInfo statusInfo;
        statusInfo.displayText = i18n("Away");
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-away"));
        statusInfo.status = User::PresenceStatus::PresenceAway;
        mStatusList.append(statusInfo);
    }
    {
        StatusInfo statusInfo;
        statusInfo.displayText = i18n("Offline");
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-offline"));
        statusInfo.status = User::PresenceStatus::PresenceOffline;
        mStatusList.append(statusInfo);
    }
}

int StatusModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mStatusList.count();
}

QVariant StatusModel::data(const QModelIndex &index, int role) const
{
    StatusInfo statusInfo = mStatusList.at(index.row());
    switch (role) {
    case StatusI18n:
        return statusInfo.displayText;
    case Status:
        return QVariant::fromValue(statusInfo.status);
    case Icon:
        return statusInfo.icon;
    case CurrentStatus:
        return mCurrentStatus;
    }
    return {};
}
