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
    struct UnreadInformation {
        QString identifier;
        QString i18nMessage;
        Q_REQUIRED_RESULT bool operator==(const UnreadInformation &other) const
        {
            return identifier == other.identifier && i18nMessage == other.i18nMessage;
        }
        Q_REQUIRED_RESULT bool operator!=(const UnreadInformation &other) const
        {
            return !operator==(other);
        }
    };

    BannerInfos();
    ~BannerInfos();

    Q_REQUIRED_RESULT const QVector<BannerInfo> &banners() const;
    void setBanners(const QVector<BannerInfo> &newBanners);

    Q_REQUIRED_RESULT UnreadInformation unreadInformation() const;

    void parseBannerInfos(const QJsonObject &object);

    void clear();

    Q_REQUIRED_RESULT int count() const;

    Q_REQUIRED_RESULT bool isEmpty() const;

    Q_REQUIRED_RESULT BannerInfo at(int index) const;

    Q_REQUIRED_RESULT bool hasUnreadBanner() const;

    Q_REQUIRED_RESULT QVector<BannerInfos::UnreadInformation> bannerUnreadInformations() const;

private:
    Q_REQUIRED_RESULT QString generateText(const BannerInfo &info) const;
    QVector<BannerInfo> mBanners;
};

Q_DECLARE_METATYPE(BannerInfos)
Q_DECLARE_METATYPE(BannerInfos::UnreadInformation)
Q_DECLARE_TYPEINFO(BannerInfos::UnreadInformation, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannerInfos &t);
