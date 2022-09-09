/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfosmodel.h"

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
    const auto info = mPersonalAccessTokenInfos.at(index.row());
    switch (role) {
    case PersonalAccessTokenInfosModel::CreateAt: {
        return info.createdAt();
    }
    case PersonalAccessTokenInfosModel::LastTokenPart: {
        return info.lastTokenPart();
    }
    case PersonalAccessTokenInfosModel::ByPassTwoFactor: {
        return info.bypassTwoFactor();
    }
    case Qt::DisplayRole:
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
