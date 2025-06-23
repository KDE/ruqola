/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfos.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
BannerInfos::BannerInfos() = default;

BannerInfos::~BannerInfos() = default;

void BannerInfos::parseBannerInfos(const QJsonObject &object)
{
    mBanners.clear();
    const QJsonObject obj = object["banners"_L1].toObject();
    const QStringList keys = obj.keys();
    for (const auto &key : keys) {
        const QJsonObject currentObj = obj[key].toObject();
        BannerInfo info;
        info.parseBannerInfo(currentObj);
        if (info.isValid()) {
            mBanners.append(std::move(info));
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

QList<BannerInfos::UnreadInformation> BannerInfos::bannerUnreadInformations() const
{
    QList<BannerInfos::UnreadInformation> infos;
    for (int i = 0; i < mBanners.size(); ++i) {
        const auto banner = mBanners.at(i);
        if (!banner.read()) {
            BannerInfos::UnreadInformation info;
            info.i18nMessage = generateText(banner);
            info.identifier = banner.identifier();
            infos.append(std::move(info));
        }
    }
    return infos;
}

QString BannerInfos::generateText(const BannerInfo &info) const
{
    QString str = BannerInfo::defaultText(info);
    if (!info.link().isEmpty()) {
        str += u" <a href=\"%1\">%2</a>"_s.arg(info.link(), i18n("(link)"));
    }
    return str;
}

void BannerInfos::clear()
{
    mBanners.clear();
}

void BannerInfos::updateBannerReadInfo(const QByteArray &name, bool readStatus)
{
    for (int i = 0, total = mBanners.size(); i < total; ++i) {
        if (mBanners.at(i).identifier() == name) {
            mBanners[i].setRead(readStatus);
        }
    }
}

int BannerInfos::count() const
{
    return mBanners.count();
}

QDebug operator<<(QDebug d, const BannerInfos &t)
{
    for (int i = 0, total = t.banners().count(); i < total; ++i) {
        d.space() << t.banners().at(i) << "\n";
    }
    return d;
}
const QList<BannerInfo> &BannerInfos::banners() const
{
    return mBanners;
}

void BannerInfos::setBanners(const QList<BannerInfo> &newBanners)
{
    mBanners = newBanners;
}
