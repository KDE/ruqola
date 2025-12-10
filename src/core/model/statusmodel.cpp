/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statusmodel.h"
using namespace Qt::Literals::StringLiterals;

#include "customusers/customuserstatus.h"
#include "utils.h"
#include <KLocalizedString>

StatusModel::StatusModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

StatusModel::~StatusModel() = default;

User::PresenceStatus StatusModel::currentUserStatus() const
{
    return mStatusList.at(mCurrentStatus).status;
}

StatusModel::StatusInfo StatusModel::currentStatusInfo() const
{
    const DisplayStatusInfo info = mStatusList.at(mCurrentStatus);
    StatusInfo currentStatusInfo;
    currentStatusInfo.status = info.status;
    currentStatusInfo.statusStr = info.statusStr;
    if (!mCustomText.isEmpty()) {
        currentStatusInfo.statusStr = mCustomText;
    }
    return currentStatusInfo;
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
    const QString statusStr = Utils::i18nFromPresenceStatus(status);
    if (statusStr.isEmpty()) {
        return i18n("Modify Status…");
    }
    return statusStr;
}

QIcon StatusModel::iconFromPresenceStatus(User::PresenceStatus status) const
{
    switch (status) {
    case User::PresenceStatus::Online:
        return QIcon::fromTheme(u"user-online"_s);
    case User::PresenceStatus::Busy:
        return QIcon::fromTheme(u"user-busy"_s);
    case User::PresenceStatus::Away:
        return QIcon::fromTheme(u"user-away"_s);
    case User::PresenceStatus::Offline:
        return QIcon::fromTheme(u"user-offline"_s);
    case User::PresenceStatus::Unknown:
        return {};
    }
    return {};
}

StatusModel::DisplayStatusInfo StatusModel::createStatusInfo(User::PresenceStatus status, int order)
{
    DisplayStatusInfo statusInfo;
    statusInfo.status = status;
    statusInfo.displayText = textFromPresenceStatus(statusInfo.status);
    statusInfo.icon = iconFromPresenceStatus(statusInfo.status);
    statusInfo.order = order;
    statusInfo.statusStr = Utils::presenceStatusToString(status);
    return statusInfo;
}

void StatusModel::fillModel()
{
    mStatusList.clear();
    {
        const DisplayStatusInfo statusInfo = createStatusInfo(User::PresenceStatus::Online, 20);
        mStatusList.append(statusInfo);
    }
    {
        const DisplayStatusInfo statusInfo = createStatusInfo(User::PresenceStatus::Busy, 19);
        mStatusList.append(statusInfo);
    }
    {
        const DisplayStatusInfo statusInfo = createStatusInfo(User::PresenceStatus::Away, 18);
        mStatusList.append(statusInfo);
    }
    {
        const DisplayStatusInfo statusInfo = createStatusInfo(User::PresenceStatus::Offline, 17);
        mStatusList.append(statusInfo);
    }
    {
        const DisplayStatusInfo statusInfo = createStatusInfo(User::PresenceStatus::Unknown, -1);
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

void StatusModel::updateCustomStatus(const QList<CustomUserStatus> &customUserStatuses)
{
    beginResetModel();
    fillModel();
    for (const CustomUserStatus &status : customUserStatuses) {
        DisplayStatusInfo statusInfo;
        statusInfo.status = status.statusType();
        statusInfo.displayText = status.name();
        statusInfo.icon = iconFromPresenceStatus(statusInfo.status);
        statusInfo.order = 5;
        statusInfo.statusStr = status.name();
        mStatusList.append(statusInfo);
    }
    endResetModel();
}

int StatusModel::currentStatus() const
{
    return mCurrentStatus;
}

int StatusModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
    return mStatusList.count();
}

QVariant StatusModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mStatusList.count()) {
        return {};
    }
    const DisplayStatusInfo statusInfo = mStatusList.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case StatusI18n:
        return statusInfo.displayText;
    case Status:
        return QVariant::fromValue(statusInfo.status);
    case Order:
        return statusInfo.order;
    case Qt::DecorationRole:
    case Icon:
        return statusInfo.icon;
    default:
        break;
    }

    return {};
}

#include "moc_statusmodel.cpp"
