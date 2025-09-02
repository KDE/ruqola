/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportedmessagemodel.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

ModerationReportedMessageModel::ModerationReportedMessageModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

ModerationReportedMessageModel::~ModerationReportedMessageModel() = default;

int ModerationReportedMessageModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mModerationInfos.count();
}

QVariant ModerationReportedMessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<ModerationReportedMessageRoles>(section)) {
        case ModerationReportedMessageRoles::Name:
            return i18n("Name");
        case ModerationReportedMessageRoles::UserId:
        case ModerationReportedMessageRoles::MessageId:
        case ModerationReportedMessageRoles::UserDeleted:
        case ModerationReportedMessageRoles::ReportDate:
            return {};
        case ModerationReportedMessageRoles::Message:
            return i18n("Reported message");
        case ModerationReportedMessageRoles::UserName:
            return i18n("Username");
        case ModerationReportedMessageRoles::Reports:
            return i18n("Reports");
        case ModerationReportedMessageRoles::ReportDateDisplay:
            return i18n("Report date");
        case ModerationReportedMessageRoles::RoomName:
            return i18n("Room");
        }
    }
    return {};
}

int ModerationReportedMessageModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(ModerationReportedMessageRoles::LastColumn) + 1;
    return val;
}

QVariant ModerationReportedMessageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mModerationInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const ModerationReportedMessageInfo &moderationReportedMessageInfo = mModerationInfos.at(index.row());
    const int col = index.column();
    switch (static_cast<ModerationReportedMessageRoles>(col)) {
    case ModerationReportedMessageRoles::UserId:
        return moderationReportedMessageInfo.userId();
    case ModerationReportedMessageRoles::Name:
        return moderationReportedMessageInfo.name();
    case ModerationReportedMessageRoles::Message:
        return moderationReportedMessageInfo.message();
    case ModerationReportedMessageRoles::UserName:
        return moderationReportedMessageInfo.userName();
    case ModerationReportedMessageRoles::MessageId:
        return moderationReportedMessageInfo.msgId();
    case ModerationReportedMessageRoles::Reports:
        return moderationReportedMessageInfo.count();
    case ModerationReportedMessageRoles::UserDeleted:
        return moderationReportedMessageInfo.isUserDeleted();
    case ModerationReportedMessageRoles::ReportDate:
        return moderationReportedMessageInfo.createdAt();
    case ModerationReportedMessageRoles::ReportDateDisplay:
        return moderationReportedMessageInfo.createAtDisplayDateTime();
    case ModerationReportedMessageRoles::RoomName:
        return moderationReportedMessageInfo.roomList().join(u',');
    }
    return {};
}

int ModerationReportedMessageModel::total() const
{
    return mModerationInfos.count();
}

void ModerationReportedMessageModel::clear()
{
    if (!mModerationInfos.isEmpty()) {
        beginResetModel();
        mModerationInfos.clear();
        endResetModel();
    }
}

void ModerationReportedMessageModel::parseElements(const QJsonObject &obj)
{
    clear();
    mModerationInfos.parseModerationInfos(obj);
    if (!mModerationInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mModerationInfos.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

void ModerationReportedMessageModel::checkFullList()
{
    setHasFullList(mModerationInfos.count() == mModerationInfos.total());
}

const ModerationReportedMessageInfos &ModerationReportedMessageModel::moderationInfos() const
{
    return mModerationInfos;
}

void ModerationReportedMessageModel::setModerationInfos(const ModerationReportedMessageInfos &newDeviceInfos)
{
    clear();
    if (!mModerationInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mModerationInfos.count() - 1);
        mModerationInfos = newDeviceInfos;
        endInsertRows();
    }
}

void ModerationReportedMessageModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mModerationInfos.count();
    mModerationInfos.parseModerationInfos(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mModerationInfos.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> ModerationReportedMessageModel::hideColumns() const
{
    return {ModerationReportedMessageRoles::UserDeleted,
            ModerationReportedMessageRoles::UserId,
            ModerationReportedMessageRoles::MessageId,
            ModerationReportedMessageRoles::ReportDate};
}

void ModerationReportedMessageModel::removeElement([[maybe_unused]] const QByteArray &identifier)
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

#include "moc_moderationreportedmessagemodel.cpp"
