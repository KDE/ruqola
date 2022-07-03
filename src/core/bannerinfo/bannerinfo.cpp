/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfo.h"

BannerInfo::BannerInfo() = default;

BannerInfo::~BannerInfo() = default;

void BannerInfo::parseBannerInfo(const QJsonObject &object)
{
    // TODO
}

bool BannerInfo::operator==(const BannerInfo &other) const
{
    // TODO
    return true;
}

QDebug operator<<(QDebug d, const BannerInfo &t)
{
    return d;
}
