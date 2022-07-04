/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "bannerinfo.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QVector>

class LIBRUQOLACORE_EXPORT BannerInfos
{
public:
    BannerInfos();
    ~BannerInfos();

    Q_REQUIRED_RESULT const QVector<BannerInfo> &banners() const;
    void setBanners(const QVector<BannerInfo> &newBanners);

private:
    QVector<BannerInfo> mBanners;
};

Q_DECLARE_METATYPE(BannerInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannerInfos &t);
