/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT BannerInfo
{
public:
    BannerInfo();
    ~BannerInfo();

    void parseBannerInfo(const QJsonObject &object);

    Q_REQUIRED_RESULT bool operator==(const BannerInfo &other) const;
};

Q_DECLARE_METATYPE(BannerInfo)
Q_DECLARE_TYPEINFO(BannerInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannerInfo &t);
