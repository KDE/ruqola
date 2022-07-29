/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfos.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QJsonObject>

BannerInfos::BannerInfos() = default;

BannerInfos::~BannerInfos() = default;

void BannerInfos::parseBannerInfos(const QJsonObject &object)
{
    mBanners.clear();
    const QJsonObject obj = object[QStringLiteral("banners")].toObject();
    const QStringList keys = obj.keys();
    for (const auto &key : keys) {
        const QJsonObject currentObj = obj[key].toObject();
        BannerInfo info;
        info.parseBannerInfo(currentObj);
        if (info.isValid()) {
            mBanners.append(info);
        }
    }
}

bool BannerInfos::isEmpty() const
{
    return mBanners.isEmpty();
}

BannerInfo BannerInfos::at(int index) const
{
    if (index < 0 || index > mBanners.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mBanners.at(index);
}

QVector<BannerInfos::UnreadInformation> BannerInfos::bannerUnreadInformations() const
{
    QVector<BannerInfos::UnreadInformation> infos;
    for (int i = 0; i < mBanners.size(); ++i) {
        const auto banner = mBanners.at(i);
        if (!banner.read()) {
            BannerInfos::UnreadInformation info;
            info.i18nMessage = generateText(banner);
            info.identifier = banner.identifier();
            infos.append(info);
        }
    }
    return infos;
}

QString BannerInfos::generateText(const BannerInfo &info) const
{
    QString str{info.text()};
    if (str == QLatin1String("New_version_available_(s)")) {
        str = i18n("New version available %1", info.textArguments().at(0));
    }
    // FIXME
    if (!info.link().isEmpty()) {
        // Use markdown url
        str += QStringLiteral(" [%1](%2)").arg(i18n("link"), info.link());
    }
    return str;
}

void BannerInfos::clear()
{
    mBanners.clear();
}

int BannerInfos::count() const
{
    return mBanners.count();
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

BannerInfos::UnreadInformation BannerInfos::unreadInformation() const
{
    // TODO
    return {};
}
