/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfosmodel.h"
#include <KLocalizedString>

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
    const auto info = mBannerInfos.at(index.row());
    switch (role) {
    case BannerInfosModel::Read: {
        return info.read();
    }
    case BannerInfosRoles::Identifier: {
        return info.identifier();
    }
    case BannerInfosRoles::Title: {
        return info.title();
    }
    case Qt::DisplayRole:
    case BannerInfosRoles::Text: {
        return text(info);
    }
    }
    return {};
}

QString BannerInfosModel::text(const BannerInfo &info) const
{
    QString str{info.text()};
    if (str == QLatin1String("New_version_available_(s)")) {
        str = i18n("New version available %1", info.textArguments().at(0));
    }
    if (!info.link().isEmpty()) {
        // Use markdown url
        str += QStringLiteral(" [%1](%2)").arg(i18n("link"), info.link());
    }
    return str;
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
