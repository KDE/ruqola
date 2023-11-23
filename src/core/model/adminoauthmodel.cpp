/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
    return mAdminOauth.count();
}

QVariant AdminOauthModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminOauthRoles>(section)) {
        case AdminOauthModel::Identifier:
            return i18n("Identifier");
        case AdminOauthModel::Name:
            return i18n("Name");
        case AdminOauthModel::ClientId:
            return i18n("Client Id");
        case AdminOauthModel::ClientSecret:
            return i18n("Client Secret");
        case AdminOauthModel::RedirectUri:
            return i18n("Redirect Url");
        case AdminOauthModel::Active:
            return i18n("Active");
        case AdminOauthModel::CreatedAt:
            return i18n("Created At");
        case AdminOauthModel::CreatedAtStr:
        case AdminOauthModel::CreatedBy:
            return i18n("Created By");
        }
    }
    return {};
}

int AdminOauthModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(AdminOauthModel::LastColumn) + 1;
    return val;
}

const QVector<OauthInfo> &AdminOauthModel::adminOauth() const
{
    return mAdminOauth;
}

void AdminOauthModel::clear()
{
    if (!mAdminOauth.isEmpty()) {
        beginResetModel();
        mAdminOauth.clear();
        endResetModel();
    }
}

void AdminOauthModel::setAdminOauth(const QVector<OauthInfo> &newAdminInvites)
{
    clear();
    if (!newAdminInvites.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newAdminInvites.count() - 1);
        mAdminOauth = newAdminInvites;
        endInsertRows();
    }
}

QVariant AdminOauthModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAdminOauth.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const OauthInfo &info = mAdminOauth.at(index.row());
    const int col = index.column();
    switch (col) {
    case AdminOauthModel::Identifier:
        return info.identifier();
    case AdminOauthModel::Name:
        return info.name();
    case AdminOauthModel::ClientId:
        return info.clientId();
    case AdminOauthModel::ClientSecret:
        return info.clientSecret();
    case AdminOauthModel::RedirectUri:
        return info.redirectUri();
    case AdminOauthModel::Active:
        return info.active();
    case AdminOauthModel::CreatedAt:
        return info.createdDateTime();
    case AdminOauthModel::CreatedBy:
        return info.createdBy();
    case AdminOauthModel::CreatedAtStr:
        return info.createdDateTime().toString();
    }
    return {};
}

void AdminOauthModel::removeOauth(const QString &identifier)
{
    const int roomCount = mAdminOauth.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mAdminOauth.at(i).identifier() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mAdminOauth.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void AdminOauthModel::addMoreOauth(const OauthInfo &info)
{
    const int numberOfElement = mAdminOauth.count();
    mAdminOauth.append(info);
    beginInsertRows(QModelIndex(), numberOfElement, numberOfElement);
    endInsertRows();
}

#include "moc_adminoauthmodel.cpp"
