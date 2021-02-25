/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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
#include "ruqola_debug.h"
#include <KLocalizedString>

StatusModel::StatusModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

StatusModel::~StatusModel()
{
}

User::PresenceStatus StatusModel::currentUserStatus() const
{
    return mStatusList.at(mCurrentStatus).status;
}

StatusInfo StatusModel::currentStatusInfo() const
{
    StatusInfo info = mStatusList.at(mCurrentStatus);
    if (!mCustomText.isEmpty()) {
        info.displayText = mCustomText;
    }
    return info;
}

User::PresenceStatus StatusModel::status(int index) const
{
    return mStatusList.at(index).status;
}

void StatusModel::setCurrentPresenceStatus(User::PresenceStatus status)
{
    int newStatusIndex = 0;
    for (int i = 0, total = mStatusList.count(); i < total; ++i) {
        if (mStatusList.at(i).status == status) {
            newStatusIndex = i;
            break;
        }
    }
    if (mCurrentStatus != newStatusIndex) {
        mCurrentStatus = newStatusIndex;
        Q_EMIT currentStatusChanged();
    }
}

QString StatusModel::textFromPresenceStatus(User::PresenceStatus status) const
{
    switch (status) {
    case User::PresenceStatus::PresenceOnline:
        return i18n("Online");
    case User::PresenceStatus::PresenceBusy:
        return i18n("Busy");
    case User::PresenceStatus::PresenceAway:
        return i18n("Away");
    case User::PresenceStatus::PresenceOffline:
        return i18n("Offline");
    case User::PresenceStatus::Unknown:
        return i18n("Modify Status...");
    }
    return {};
}

void StatusModel::fillModel()
{
    {
        StatusInfo statusInfo;
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-online"));
        statusInfo.status = User::PresenceStatus::PresenceOnline;
        statusInfo.displayText = textFromPresenceStatus(statusInfo.status);
        statusInfo.order = 20;
        mStatusList.append(statusInfo);
    }
    {
        StatusInfo statusInfo;
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-busy"));
        statusInfo.status = User::PresenceStatus::PresenceBusy;
        statusInfo.displayText = textFromPresenceStatus(statusInfo.status);
        statusInfo.order = 19;
        mStatusList.append(statusInfo);
    }
    {
        StatusInfo statusInfo;
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-away"));
        statusInfo.status = User::PresenceStatus::PresenceAway;
        statusInfo.displayText = textFromPresenceStatus(statusInfo.status);
        statusInfo.order = 18;
        mStatusList.append(statusInfo);
    }
    {
        StatusInfo statusInfo;
        statusInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-offline"));
        statusInfo.status = User::PresenceStatus::PresenceOffline;
        statusInfo.displayText = textFromPresenceStatus(statusInfo.status);
        statusInfo.order = 17;
        mStatusList.append(statusInfo);
    }
    {
        StatusInfo statusInfo;
        statusInfo.status = User::PresenceStatus::Unknown;
        statusInfo.displayText = textFromPresenceStatus(statusInfo.status);
        statusInfo.order = -1; // Last one
        mStatusList.append(statusInfo);
    }
}

QString StatusModel::customText() const
{
    return mCustomText;
}

void StatusModel::setCustomText(const QString &customText)
{
    mCustomText = customText;
}

int StatusModel::currentStatus() const
{
    return mCurrentStatus;
}

int StatusModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mStatusList.count();
}

QVariant StatusModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mStatusList.count()) {
        return {};
    }
    const StatusInfo statusInfo = mStatusList.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case StatusI18n:
        return statusInfo.displayText;
    case Status:
        return QVariant::fromValue(statusInfo.status);
    case Qt::DecorationRole:
    case Icon:
        return statusInfo.icon;
    }

    return {};
}
