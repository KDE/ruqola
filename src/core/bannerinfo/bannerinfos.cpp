/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfos.h"

BannerInfos::BannerInfos() = default;

BannerInfos::~BannerInfos() = default;

QDebug operator<<(QDebug d, const BannerInfos &t)
{
    //    d << "total " << t.total();
    //    d << "offset " << t.offset();
    //    d << "roomsCount " << t.roomsCount() << "\n";
    //    for (int i = 0, total = t.rooms().count(); i < total; ++i) {
    //        d << t.rooms().at(i) << "\n";
    //    }
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
