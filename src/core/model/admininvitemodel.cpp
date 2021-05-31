/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "admininvitemodel.h"
#include <KLocalizedString>

AdminInviteModel::AdminInviteModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminInviteModel::~AdminInviteModel()
{
}

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
            break;
        case AdminInviteModel::Identifier:
            return i18n("Token");
        case AdminInviteModel::Create:
            return i18n("Created at");
        case AdminInviteModel::Uses:
            return i18n("Uses");
        case AdminInviteModel::MaxUses:
            return i18n("Uses left");
        case AdminInviteModel::Expire:
            return i18n("Expiration");
        }
    }
    return QVariant();
}

int AdminInviteModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminInviteModel::LastColumn) + 1;
}

const QVector<InviteInfo> &AdminInviteModel::adminInvites() const
{
    return mAdminInvites;
}

void AdminInviteModel::setAdminInvites(const QVector<InviteInfo> &newAdminInvites)
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
    case AdminInviteModel::Create:
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
