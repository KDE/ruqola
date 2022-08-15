/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfomodel.h"
#include <KLocalizedString>

DeviceInfoModel::DeviceInfoModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DeviceInfoModel::~DeviceInfoModel() = default;

int DeviceInfoModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mDeviceInfos.count();
}

QVariant DeviceInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int DeviceInfoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(DeviceInfoRoles::LastColumn) + 1;
}

QVariant DeviceInfoModel::data(const QModelIndex &index, int role) const
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
        return deviceInfo.loginAt();
    case DeviceInfoRoles::Os:
        return deviceInfo.os();
    case DeviceInfoRoles::Client:
        return deviceInfo.client();
    }
    return {};
}

int DeviceInfoModel::total() const
{
    return mDeviceInfos.count();
}

void DeviceInfoModel::parseElements(const QJsonObject &obj)
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

void DeviceInfoModel::checkFullList()
{
    setHasFullList(mDeviceInfos.count() == mDeviceInfos.total());
}

const DeviceInfos &DeviceInfoModel::deviceInfos() const
{
    return mDeviceInfos;
}

void DeviceInfoModel::setDeviceInfos(const DeviceInfos &newDeviceInfos)
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

void DeviceInfoModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mDeviceInfos.count();
    mDeviceInfos.parseDeviceInfos(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mDeviceInfos.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> DeviceInfoModel::hideColumns() const
{
    return {DeviceInfoRoles::Identifier, DeviceInfoRoles::UserId, DeviceInfoRoles::SessionId};
}

void DeviceInfoModel::removeElement(const QString &identifier)
{
    const int userCount = mDeviceInfos.count();
    for (int i = 0; i < userCount; ++i) {
        if (mDeviceInfos.at(i).identifier() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mDeviceInfos.takeAt(i);
            mDeviceInfos.setTotal(mDeviceInfos.count()); // Update total
            endRemoveRows();
            Q_EMIT totalChanged();
            break;
        }
    }
}
