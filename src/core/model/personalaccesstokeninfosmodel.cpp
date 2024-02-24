/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

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
    case PersonalAccessTokenInfosModel::CreateAtDateTime: {
        return info.createdAt();
    }
    case PersonalAccessTokenInfosModel::LastTokenPart: {
        return QString(QStringLiteral("...") + info.lastTokenPart());
    }
    case PersonalAccessTokenInfosModel::ByPassTwoFactor: {
        return info.bypassTwoFactor() ? i18n("Ignored") : i18n("Required");
    }
    case PersonalAccessTokenInfosModel::Name: {
        return info.name();
    }
    }
    return {};
}

void PersonalAccessTokenInfosModel::clear()
{
    if (!mPersonalAccessTokenInfos.isEmpty()) {
        beginResetModel();
        mPersonalAccessTokenInfos.clear();
        endResetModel();
    }
}

void PersonalAccessTokenInfosModel::insertPersonalAccessTokenInfos(const PersonalAccessTokenInfos &infos)
{
    clear();
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
        case PersonalAccessTokenInfosModel::CreateAtDateTime:
            return {};
        }
    }
    return {};
}

int PersonalAccessTokenInfosModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(PersonalAccessTokenInfosModel::LastColumn) + 1;
    return val;
}

void PersonalAccessTokenInfosModel::removeToken(const QString &tokenName)
{
    const int roomCount = mPersonalAccessTokenInfos.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mPersonalAccessTokenInfos.at(i).name() == tokenName) {
            beginRemoveRows(QModelIndex(), i, i);
            mPersonalAccessTokenInfos.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

#include "moc_personalaccesstokeninfosmodel.cpp"
