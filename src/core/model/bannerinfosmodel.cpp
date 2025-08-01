/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfosmodel.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

BannerInfosModel::BannerInfosModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

BannerInfosModel::~BannerInfosModel() = default;

int BannerInfosModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
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
    QString str = BannerInfo::defaultText(info);
    if (!info.link().isEmpty()) {
        // Use markdown url
        str += u" [%1](%2)"_s.arg(i18n("link"), info.link());
    }
    return str;
}

void BannerInfosModel::clear()
{
    if (rowCount() != 0) {
        beginResetModel();
        mBannerInfos.clear();
        endResetModel();
    }
}

void BannerInfosModel::insertBannerInfos(const BannerInfos &infos)
{
    clear();
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mBannerInfos = infos;
        endInsertRows();
    }
}

#include "moc_bannerinfosmodel.cpp"
