/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportedusermodel.h"
#include <KLocalizedString>

ModerationReportedUserModel::ModerationReportedUserModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

ModerationReportedUserModel::~ModerationReportedUserModel() = default;

int ModerationReportedUserModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mModerationInfos.count();
}

QVariant ModerationReportedUserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<ModerationReportedUserRoles>(section)) {
        case ModerationReportedUserRoles::Reports:
            return i18n("Reports");
        case ModerationReportedUserRoles::ReportDateDisplay:
            return i18n("Report date");
        case ModerationReportedUserRoles::Identifier:
        case ModerationReportedUserRoles::ReportDate:
            return {};
        case ModerationReportedUserRoles::UserName:
            return i18n("Username");
        case ModerationReportedUserRoles::Name:
            return i18n("Name");
        }
    }
    return {};
}

int ModerationReportedUserModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(ModerationReportedUserRoles::LastColumn) + 1;
    return val;
}

QVariant ModerationReportedUserModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mModerationInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }
    const ModerationReportedUserInfo &moderationReportedUserInfo = mModerationInfos.at(index.row());
    const int col = index.column();
    switch (static_cast<ModerationReportedUserRoles>(col)) {
    case ModerationReportedUserRoles::ReportDate:
        return moderationReportedUserInfo.createdAt();
    case ModerationReportedUserRoles::ReportDateDisplay:
        return moderationReportedUserInfo.createAtDisplayDateTime();
    case ModerationReportedUserRoles::Reports:
        return moderationReportedUserInfo.count();
    case ModerationReportedUserRoles::UserName:
        return moderationReportedUserInfo.username();
    case ModerationReportedUserRoles::Name:
        return moderationReportedUserInfo.name();
    case ModerationReportedUserRoles::Identifier:
        return moderationReportedUserInfo.identifier();
    }
    return {};
}

int ModerationReportedUserModel::total() const
{
    return mModerationInfos.count();
}

void ModerationReportedUserModel::clear()
{
    if (!mModerationInfos.isEmpty()) {
        beginResetModel();
        mModerationInfos.clear();
        endResetModel();
    }
}

void ModerationReportedUserModel::parseElements(const QJsonObject &obj)
{
    beginResetModel();
    mModerationInfos.clear();
    mModerationInfos.parseModerationReportedUserInfos(obj);
    endResetModel();
    checkFullList();
    Q_EMIT totalChanged();
}

void ModerationReportedUserModel::checkFullList()
{
    setHasFullList(mModerationInfos.count() == mModerationInfos.total());
}

const ModerationReportedUserInfos &ModerationReportedUserModel::moderationInfos() const
{
    return mModerationInfos;
}

void ModerationReportedUserModel::setModerationInfos(ModerationReportedUserInfos newDeviceInfos)
{
    beginResetModel();
    mModerationInfos = std::move(newDeviceInfos);
    endResetModel();
}

void ModerationReportedUserModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mModerationInfos.count();
    ModerationReportedUserInfos moderationInfos = mModerationInfos;
    moderationInfos.parseMoreModerationReportedUserInfos(obj);
    const int newNumberOfElement = moderationInfos.count();
    if (newNumberOfElement > numberOfElement) {
        beginInsertRows(QModelIndex(), numberOfElement, newNumberOfElement - 1);
        mModerationInfos = std::move(moderationInfos);
        endInsertRows();
    } else { // No new element but offset/total may have changed
        mModerationInfos = std::move(moderationInfos);
    }
    checkFullList();
}

QList<int> ModerationReportedUserModel::hideColumns() const
{
    return {ModerationReportedUserRoles::ReportDate, ModerationReportedUserRoles::Identifier};
}

void ModerationReportedUserModel::removeElement([[maybe_unused]] const QByteArray &identifier)
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

#include "moc_moderationreportedusermodel.cpp"
