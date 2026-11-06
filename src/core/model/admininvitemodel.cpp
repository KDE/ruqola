/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

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
            return i18n("Max uses");
        case AdminInviteModel::Expire:
            return i18n("Expiration");
        }
    }
    return {};
}

int AdminInviteModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(AdminInviteModel::LastColumn) + 1;
    return val;
}

const QList<InviteInfo> &AdminInviteModel::adminInvites() const
{
    return mAdminInvites;
}

void AdminInviteModel::setAdminInvites(QList<InviteInfo> newAdminInvites)
{
    beginResetModel();
    mAdminInvites = std::move(newAdminInvites);
    endResetModel();
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
        return QLocale().toString(inviteInfo.createDateTime());
    case AdminInviteModel::Uses:
        return inviteInfo.uses();
    case AdminInviteModel::MaxUses:
        return maxUses(inviteInfo.maxUses());
    case AdminInviteModel::Expire:
        return expireInvitation(inviteInfo);
    default:
        break;
    }
    return {};
}

QString AdminInviteModel::maxUses(int uses)
{
    if (uses == 0) {
        return i18n("Unlimited");
    }
    return QString::number(uses);
}

QString AdminInviteModel::expireInvitation(const InviteInfo &inviteInfo)
{
    const QDateTime &expireDateTime = inviteInfo.expireDateTime();
    if (!expireDateTime.isValid()) {
        return i18n("Never");
    }
    const QDateTime now = QDateTime::currentDateTime();
    if (expireDateTime > now) {
        const qint64 days = now.daysTo(expireDateTime);
        if (days == 0) {
            return i18n("Expires today");
        }
        return i18np("Expires in 1 day", "Expires in %1 days", days);
    }
    return i18n("Expired");
}

void AdminInviteModel::removeInvite(const QByteArray &identifier)
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
