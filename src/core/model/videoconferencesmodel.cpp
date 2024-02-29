/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencesmodel.h"
#include <KLocalizedString>

VideoConferencesModel::VideoConferencesModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

VideoConferencesModel::~VideoConferencesModel() = default;

int VideoConferencesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mDeviceInfos.count();
}

QVariant VideoConferencesModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int VideoConferencesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(DeviceInfoRoles::LastColumn) + 1;
    return val;
}

QVariant VideoConferencesModel::data(const QModelIndex &index, int role) const
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

int VideoConferencesModel::total() const
{
    return mDeviceInfos.count();
}

void VideoConferencesModel::clear()
{
    if (!mDeviceInfos.isEmpty()) {
        beginResetModel();
        mDeviceInfos.clear();
        endResetModel();
    }
}

void VideoConferencesModel::parseElements(const QJsonObject &obj)
{
    clear();
    mDeviceInfos.parseDeviceInfos(obj);
    if (!mDeviceInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mDeviceInfos.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

void VideoConferencesModel::checkFullList()
{
    setHasFullList(mDeviceInfos.count() == mDeviceInfos.total());
}

const DeviceInfos &VideoConferencesModel::deviceInfos() const
{
    return mDeviceInfos;
}

void VideoConferencesModel::setDeviceInfos(const DeviceInfos &newDeviceInfos)
{
    clear();
    if (!mDeviceInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mDeviceInfos.count() - 1);
        mDeviceInfos = newDeviceInfos;
        endInsertRows();
    }
}

void VideoConferencesModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mDeviceInfos.count();
    mDeviceInfos.parseDeviceInfos(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mDeviceInfos.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> VideoConferencesModel::hideColumns() const
{
    return {DeviceInfoRoles::Identifier, DeviceInfoRoles::UserId, DeviceInfoRoles::SessionId};
}

void VideoConferencesModel::removeElement(const QString &identifier)
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

#include "moc_videoconferencesmodel.cpp"
