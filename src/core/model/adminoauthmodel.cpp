/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminoauthmodel.h"
#include <KLocalizedString>
#include <QDateTime>

AdminOauthModel::AdminOauthModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminOauthModel::~AdminOauthModel() = default;

int AdminOauthModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mAdminInvites.count();
}

QVariant AdminOauthModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminInviteRoles>(section)) {
        case AdminOauthModel::UserIdentifier:
        case AdminOauthModel::RoomId:
            break;
        case AdminOauthModel::Identifier:
            return i18n("Token");
        case AdminOauthModel::CreateStr:
        case AdminOauthModel::Create:
            return i18n("Created at");
        case AdminOauthModel::Uses:
            return i18n("Uses");
        case AdminOauthModel::MaxUses:
            return i18n("Uses left");
        case AdminOauthModel::Expire:
            return i18n("Expiration");
        }
    }
    return {};
}

int AdminOauthModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminOauthModel::LastColumn) + 1;
}

const QVector<InviteInfo> &AdminOauthModel::adminInvites() const
{
    return mAdminInvites;
}

void AdminOauthModel::setAdminInvites(const QVector<InviteInfo> &newAdminInvites)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAdminInvites.count() - 1);
        mAdminInvites.clear();
        endRemoveRows();
    }
    if (!newAdminInvites.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newAdminInvites.count() - 1);
        mAdminInvites = newAdminInvites;
        endInsertRows();
    }
}

QVariant AdminOauthModel::data(const QModelIndex &index, int role) const
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
    case AdminOauthModel::UserIdentifier:
        return inviteInfo.userIdentifier();
    case AdminOauthModel::Identifier:
        return inviteInfo.identifier();
    case AdminOauthModel::RoomId:
        return inviteInfo.roomId();
    case AdminOauthModel::Create:
        return inviteInfo.createDateTime();
    case AdminOauthModel::CreateStr:
        return inviteInfo.createDateTime().toString();
    case AdminOauthModel::Uses:
        return inviteInfo.uses();
    case AdminOauthModel::MaxUses:
        return inviteInfo.maxUses();
    case AdminOauthModel::Expire:
        return expireInvitation(inviteInfo);
    }
    return {};
}

QString AdminOauthModel::expireInvitation(const InviteInfo &inviteInfo) const
{
    if (inviteInfo.expireDateTime() > QDateTime::currentDateTime()) {
        return i18n("Expire in %1 days", QDateTime::currentDateTime().daysTo(inviteInfo.expireDateTime()));
    } else {
        return i18n("Expired");
    }
}

void AdminOauthModel::removeInvite(const QString &identifier)
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
