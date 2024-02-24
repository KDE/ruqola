/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "admininvitemodel.h"
#include <KLocalizedString>
#include <QDateTime>

AdminInviteModel::AdminInviteModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminInviteModel::~AdminInviteModel() = default;

int AdminInviteModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mAdminInvites.count();
}

QVariant AdminInviteModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminInviteRoles>(section)) {
        case AdminInviteModel::UserIdentifier:
        case AdminInviteModel::RoomId:
        case AdminInviteModel::CreateAt:
            break;
        case AdminInviteModel::Identifier:
            return i18n("Token");
        case AdminInviteModel::CreateAtStr:
            return i18n("Created at");
        case AdminInviteModel::Uses:
            return i18n("Uses");
        case AdminInviteModel::MaxUses:
            return i18n("Uses left");
        case AdminInviteModel::Expire:
            return i18n("Expiration");
        }
    }
    return {};
}

int AdminInviteModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(AdminInviteModel::LastColumn) + 1;
    return val;
}

const QList<InviteInfo> &AdminInviteModel::adminInvites() const
{
    return mAdminInvites;
}

void AdminInviteModel::clear()
{
    if (!mAdminInvites.isEmpty()) {
        beginResetModel();
        mAdminInvites.clear();
        endResetModel();
    }
}

void AdminInviteModel::setAdminInvites(const QList<InviteInfo> &newAdminInvites)
{
    clear();
    if (!newAdminInvites.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newAdminInvites.count() - 1);
        mAdminInvites = newAdminInvites;
        endInsertRows();
    }
}

QVariant AdminInviteModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAdminInvites.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const InviteInfo &inviteInfo = mAdminInvites.at(index.row());
    const int col = index.column();
    switch (col) {
    case AdminInviteModel::UserIdentifier:
        return inviteInfo.userIdentifier();
    case AdminInviteModel::Identifier:
        return inviteInfo.identifier();
    case AdminInviteModel::RoomId:
        return inviteInfo.roomId();
    case AdminInviteModel::CreateAt:
        return inviteInfo.createDateTime();
    case AdminInviteModel::CreateAtStr:
        return inviteInfo.createDateTime().toString();
    case AdminInviteModel::Uses:
        return inviteInfo.uses();
    case AdminInviteModel::MaxUses:
        return inviteInfo.maxUses();
    case AdminInviteModel::Expire:
        return expireInvitation(inviteInfo);
    }
    return {};
}

QString AdminInviteModel::expireInvitation(const InviteInfo &inviteInfo) const
{
    if (inviteInfo.expireDateTime() > QDateTime::currentDateTime()) {
        return i18n("Expire in %1 days", QDateTime::currentDateTime().daysTo(inviteInfo.expireDateTime()));
    } else {
        return i18n("Expired");
    }
}

void AdminInviteModel::removeInvite(const QString &identifier)
{
    const int roomCount = mAdminInvites.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mAdminInvites.at(i).identifier() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mAdminInvites.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

#include "moc_admininvitemodel.cpp"
