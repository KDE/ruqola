/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmodel.h"
#include <KLocalizedString>

ModerationModel::ModerationModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

ModerationModel::~ModerationModel() = default;

int ModerationModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mDeviceInfos.count();
}

QVariant ModerationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<DeviceInfoRoles>(section)) {
        case DeviceInfoRoles::Identifier:
            return {};
        case DeviceInfoRoles::Host:
            return i18n("Host");
        case DeviceInfoRoles::Os:
            return i18n("Os");
        case DeviceInfoRoles::Client:
            return i18n("Client");
        case DeviceInfoRoles::SessionId:
            return i18n("Session Id");
        case DeviceInfoRoles::Ip:
            return i18n("Ip");
        case DeviceInfoRoles::UserId:
            return {};
        case DeviceInfoRoles::LoginAt:
            return i18n("Login At");
        }
    }
    return {};
}

int ModerationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(DeviceInfoRoles::LastColumn) + 1;
}

QVariant ModerationModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mDeviceInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const DeviceInfo &deviceInfo = mDeviceInfos.at(index.row());
    const int col = index.column();
    switch (static_cast<DeviceInfoRoles>(col)) {
    case DeviceInfoRoles::Identifier:
        return deviceInfo.identifier();
    case DeviceInfoRoles::Host:
        return deviceInfo.host();
    case DeviceInfoRoles::SessionId:
        return deviceInfo.sessionId();
    case DeviceInfoRoles::Ip:
        return deviceInfo.ip();
    case DeviceInfoRoles::UserId:
        return deviceInfo.userId();
    case DeviceInfoRoles::LoginAt:
        return deviceInfo.loginAtDisplay();
    case DeviceInfoRoles::Os:
        return deviceInfo.os();
    case DeviceInfoRoles::Client:
        return deviceInfo.client();
    }
    return {};
}

int ModerationModel::total() const
{
    return mDeviceInfos.count();
}

void ModerationModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mDeviceInfos.count() - 1);
        mDeviceInfos.clear();
        endRemoveRows();
    }
    mDeviceInfos.parseDeviceInfos(obj);
    if (!mDeviceInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mDeviceInfos.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

void ModerationModel::checkFullList()
{
    setHasFullList(mDeviceInfos.count() == mDeviceInfos.total());
}

const DeviceInfos &ModerationModel::deviceInfos() const
{
    return mDeviceInfos;
}

void ModerationModel::setDeviceInfos(const DeviceInfos &newDeviceInfos)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mDeviceInfos.count() - 1);
        mDeviceInfos.clear();
        endRemoveRows();
    }
    if (!mDeviceInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mDeviceInfos.count() - 1);
        mDeviceInfos = newDeviceInfos;
        endInsertRows();
    }
}

void ModerationModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mDeviceInfos.count();
    mDeviceInfos.parseDeviceInfos(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mDeviceInfos.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> ModerationModel::hideColumns() const
{
    return {DeviceInfoRoles::Identifier, DeviceInfoRoles::UserId, DeviceInfoRoles::SessionId};
}

void ModerationModel::removeElement(const QString &identifier)
{
    const int userCount = mDeviceInfos.count();
    for (int i = 0; i < userCount; ++i) {
        if (mDeviceInfos.at(i).sessionId() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mDeviceInfos.takeAt(i);
            mDeviceInfos.setTotal(mDeviceInfos.count()); // Update total
            endRemoveRows();
            Q_EMIT totalChanged();
            break;
        }
    }
}

#include "moc_moderationmodel.cpp"
