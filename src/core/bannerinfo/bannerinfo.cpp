/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfo.h"

#include <KLocalizedString>
#include <QJsonArray>

BannerInfo::BannerInfo() = default;

BannerInfo::~BannerInfo() = default;

bool BannerInfo::isValid() const
{
    return !mText.isEmpty() && !mIdentifier.isEmpty() && !mTitle.isEmpty();
}

void BannerInfo::parseBannerInfo(const QJsonObject &object)
{
    QStringList lst;
    const QJsonArray array = object[QStringLiteral("textArguments")].toArray();
    lst.reserve(array.count());
    for (const QJsonValue &current : array) {
        lst.append(current.toString());
    }
    mTextArguments = lst;
    mText = object[QStringLiteral("text")].toString();
    mTitle = object[QStringLiteral("title")].toString();
    mLink = object[QStringLiteral("link")].toString();
    mIdentifier = object[QStringLiteral("id")].toString();
    mPriority = object[QStringLiteral("priority")].toInt(-1);
    mRead = object[QStringLiteral("read")].toBool(false);
}

QString BannerInfo::defaultText(const BannerInfo &info)
{
    QString str{info.text()};
    if (str == QLatin1String("New_version_available_(s)")) {
        str = i18n("New version available %1", info.textArguments().at(0));
    }
    return str;
}

bool BannerInfo::operator==(const BannerInfo &other) const
{
    return mText == other.text() && mTitle == other.title() && mLink == other.link() && mRead == other.read() && mRead == other.read()
        && mTextArguments == other.textArguments() && mIdentifier == other.identifier() && mPriority == other.priority();
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

const QStringList &BannerInfo::textArguments() const
{
    return mTextArguments;
}

void BannerInfo::setTextArguments(const QStringList &newTextArguments)
{
    mTextArguments = newTextArguments;
}

const QString &BannerInfo::identifier() const
{
    return mIdentifier;
}

void BannerInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

int BannerInfo::priority() const
{
    return mPriority;
}

void BannerInfo::setPriority(int newPriority)
{
    mPriority = newPriority;
}

QDebug operator<<(QDebug d, const BannerInfo &t)
{
    d << " mText " << t.text();
    d << " mTitle " << t.title();
    d << " mLink " << t.link();
    d << " mRead " << t.read();
    d << " mTextArguments " << t.textArguments();
    d << " mIdentifier " << t.identifier();
    d << " mPriority " << t.priority();
    return d;
}
