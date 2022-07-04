/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfos.h"

#include <QJsonObject>

BannerInfos::BannerInfos() = default;

BannerInfos::~BannerInfos() = default;

void BannerInfos::parseBannerInfos(const QJsonObject &object)
{
    // TODO
}

QDebug operator<<(QDebug d, const BannerInfos &t)
{
    for (int i = 0, total = t.banners().count(); i < total; ++i) {
        d << t.banners().at(i) << "\n";
    }
    return d;
}
const QVector<BannerInfo> &BannerInfos::banners() const
{
    return mBanners;
}

void BannerInfos::setBanners(const QVector<BannerInfo> &newBanners)
{
    mBanners = newBanners;
}
