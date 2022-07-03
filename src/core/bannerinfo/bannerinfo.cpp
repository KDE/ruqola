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
    return mText == other.text() && mTitle == other.title() && mLink == other.link() && mRead == other.read() && mRead == other.read();
}

const QString &BannerInfo::text() const
{
    return mText;
}

void BannerInfo::setText(const QString &newText)
{
    mText = newText;
}

const QString &BannerInfo::title() const
{
    return mTitle;
}

void BannerInfo::setTitle(const QString &newTitle)
{
    mTitle = newTitle;
}

const QString &BannerInfo::link() const
{
    return mLink;
}

void BannerInfo::setLink(const QString &newLink)
{
    mLink = newLink;
}

bool BannerInfo::read() const
{
    return mRead;
}

void BannerInfo::setRead(bool newRead)
{
    mRead = newRead;
}

QDebug operator<<(QDebug d, const BannerInfo &t)
{
    d << "mText " << t.text();
    d << "mTitle " << t.title();
    d << "mLink " << t.link();
    d << "mRead " << t.read();
    return d;
}
