/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "bannerinfo.h"
#include "libruqolacore_export.h"
#include <QDebug>
#include <QVector>

class LIBRUQOLACORE_EXPORT BannerInfos
{
public:
    struct UnreadInformation {
        QString identifier;
        QString i18nMessage;
        [[nodiscard]] bool operator==(const UnreadInformation &other) const
        {
            return identifier == other.identifier && i18nMessage == other.i18nMessage;
        }
        [[nodiscard]] bool operator!=(const UnreadInformation &other) const
        {
            return !operator==(other);
        }
    };

    BannerInfos();
    ~BannerInfos();

    [[nodiscard]] const QVector<BannerInfo> &banners() const;
    void setBanners(const QVector<BannerInfo> &newBanners);

    void parseBannerInfos(const QJsonObject &object);

    void clear();

    void updateBannerReadInfo(const QString &name, bool readStatus);

    [[nodiscard]] int count() const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] BannerInfo at(int index) const;

    [[nodiscard]] QVector<BannerInfos::UnreadInformation> bannerUnreadInformations() const;

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateText(const BannerInfo &info) const;
    QVector<BannerInfo> mBanners;
};

Q_DECLARE_METATYPE(BannerInfos)
Q_DECLARE_METATYPE(BannerInfos::UnreadInformation)
Q_DECLARE_TYPEINFO(BannerInfos::UnreadInformation, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannerInfos &t);
