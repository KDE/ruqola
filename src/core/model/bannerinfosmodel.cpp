/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfosmodel.h"

BannerInfosModel::BannerInfosModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

BannerInfosModel::~BannerInfosModel() = default;

int BannerInfosModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mBannerInfos.count();
}

QVariant BannerInfosModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mBannerInfos.count()) {
        return {};
    }
#if 0
    const auto info = mNotificationInfo.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case AccountName:
        return info.accountName();
    }
#endif
    return {};
}

void BannerInfosModel::clear()
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mBannerInfos.count() - 1);
        mBannerInfos.clear();
        endRemoveRows();
    }
}

void BannerInfosModel::insertBannerInfos(const BannerInfos &infos)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mBannerInfos.count() - 1);
        mBannerInfos.clear();
        endRemoveRows();
    }
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mBannerInfos = infos;
        endInsertRows();
    }
}
