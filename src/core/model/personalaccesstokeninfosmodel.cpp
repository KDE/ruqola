/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfosmodel.h"
#include <KLocalizedString>

PersonalAccessTokenInfosModel::PersonalAccessTokenInfosModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

PersonalAccessTokenInfosModel::~PersonalAccessTokenInfosModel() = default;

int PersonalAccessTokenInfosModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mPersonalAccessTokenInfos.count();
}

QVariant PersonalAccessTokenInfosModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mPersonalAccessTokenInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const auto &info = mPersonalAccessTokenInfos.at(index.row());
    const int col = index.column();
    switch (col) {
    case PersonalAccessTokenInfosModel::CreateAt: {
        return info.createAtDisplayDateTime();
    }
    case PersonalAccessTokenInfosModel::LastTokenPart: {
        return info.lastTokenPart();
    }
    case PersonalAccessTokenInfosModel::ByPassTwoFactor: {
        return info.bypassTwoFactor() ? i18n("Required") : i18n("Ignored");
    }
    case PersonalAccessTokenInfosModel::Name: {
        return info.name();
    }
    }
    return {};
}

void PersonalAccessTokenInfosModel::clear()
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mPersonalAccessTokenInfos.count() - 1);
        mPersonalAccessTokenInfos.clear();
        endRemoveRows();
    }
}

void PersonalAccessTokenInfosModel::insertPersonalAccessTokenInfos(const PersonalAccessTokenInfos &infos)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mPersonalAccessTokenInfos.count() - 1);
        mPersonalAccessTokenInfos.clear();
        endRemoveRows();
    }
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mPersonalAccessTokenInfos = infos;
        endInsertRows();
    }
}

QVariant PersonalAccessTokenInfosModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<PersonalAccessTokenInfosRoles>(section)) {
        case PersonalAccessTokenInfosModel::Name:
            return i18n("Name");
        case PersonalAccessTokenInfosModel::CreateAt:
            return i18n("Create At");
        case PersonalAccessTokenInfosModel::ByPassTwoFactor:
            return i18n("Two Factor Authentication");
        case PersonalAccessTokenInfosModel::LastTokenPart:
            return i18n("Last token part");
        }
    }
    return {};
}

int PersonalAccessTokenInfosModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(PersonalAccessTokenInfosModel::LastColumn) + 1;
}
