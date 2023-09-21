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
    return mModerationInfos.count();
}

QVariant ModerationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<ModerationInfoRoles>(section)) {
        case ModerationInfoRoles::Name:
            return i18n("Name");
        case ModerationInfoRoles::UserId:
        case ModerationInfoRoles::MessageId:
        case ModerationInfoRoles::UserDeleted:
            return {};
        case ModerationInfoRoles::Message:
            return i18n("Reported message");
        case ModerationInfoRoles::UserName:
            return i18n("Username");
        case ModerationInfoRoles::Reports:
            return i18n("Reports");
        case ModerationInfoRoles::ReportDate:
            return i18n("Report date");
        case ModerationInfoRoles::RoomName:
            return i18n("Room");
        }
    }
    return {};
}

int ModerationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(ModerationInfoRoles::LastColumn) + 1;
}

QVariant ModerationModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mModerationInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const ModerationInfo &moderationInfo = mModerationInfos.at(index.row());
    const int col = index.column();
    switch (static_cast<ModerationInfoRoles>(col)) {
    case ModerationInfoRoles::UserId:
        return moderationInfo.userId();
    case ModerationInfoRoles::Name:
        return moderationInfo.name();
    case ModerationInfoRoles::Message:
        return moderationInfo.message();
    case ModerationInfoRoles::UserName:
        return moderationInfo.userName();
    case ModerationInfoRoles::MessageId:
        return moderationInfo.msgId();
    case ModerationInfoRoles::Reports:
        return moderationInfo.count();
    case ModerationInfoRoles::UserDeleted:
        return moderationInfo.isUserDeleted();
    case ModerationInfoRoles::ReportDate:
        return moderationInfo.createAtDisplayDateTime();
    case ModerationInfoRoles::RoomName:
        return {}; // TODO
    }
    return {};
}

int ModerationModel::total() const
{
    return mModerationInfos.count();
}

void ModerationModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mModerationInfos.count() - 1);
        mModerationInfos.clear();
        endRemoveRows();
    }
    mModerationInfos.parseModerationInfos(obj);
    if (!mModerationInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mModerationInfos.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

void ModerationModel::checkFullList()
{
    setHasFullList(mModerationInfos.count() == mModerationInfos.total());
}

const ModerationInfos &ModerationModel::moderationInfos() const
{
    return mModerationInfos;
}

void ModerationModel::setModerationInfos(const ModerationInfos &newDeviceInfos)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mModerationInfos.count() - 1);
        mModerationInfos.clear();
        endRemoveRows();
    }
    if (!mModerationInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mModerationInfos.count() - 1);
        mModerationInfos = newDeviceInfos;
        endInsertRows();
    }
}

void ModerationModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mModerationInfos.count();
    mModerationInfos.parseModerationInfos(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mModerationInfos.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> ModerationModel::hideColumns() const
{
    return {ModerationInfoRoles::UserDeleted, ModerationInfoRoles::UserId, ModerationInfoRoles::MessageId};
}

void ModerationModel::removeElement(const QString &identifier)
{
#if 0
    const int userCount = mModerationInfos.count();
    for (int i = 0; i < userCount; ++i) {
        if (mModerationInfos.at(i).sessionId() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mModerationInfos.takeAt(i);
            mModerationInfos.setTotal(mModerationInfos.count()); // Update total
            endRemoveRows();
            Q_EMIT totalChanged();
            break;
        }
    }
#endif
}

#include "moc_moderationmodel.cpp"
