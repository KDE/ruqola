/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfo.h"

#include <KLocalizedString>
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;
BannerInfo::BannerInfo() = default;

BannerInfo::~BannerInfo() = default;

bool BannerInfo::isValid() const
{
    return !mText.isEmpty() && !mIdentifier.isEmpty() && !mTitle.isEmpty();
}

void BannerInfo::parseBannerInfo(const QJsonObject &object)
{
    QStringList lst;
    const QJsonArray array = object["textArguments"_L1].toArray();
    lst.reserve(array.count());
    for (const QJsonValue &current : array) {
        lst.append(current.toString());
    }
    mTextArguments = lst;
    mText = object["text"_L1].toString();
    mTitle = object["title"_L1].toString();
    mLink = object["link"_L1].toString();
    mIdentifier = object["id"_L1].toString().toLatin1();
    mPriority = object["priority"_L1].toInt(-1);
    mRead = object["read"_L1].toBool(false);
    //    if (mPriority != -1) {
    //        qWarning() << " priority != -1 " << object;
    //    }
}

QString BannerInfo::defaultText(const BannerInfo &info)
{
    QString str{info.text()};
    if (str == "New_version_available_(s)"_L1) {
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

const QByteArray &BannerInfo::identifier() const
{
    return mIdentifier;
}

void BannerInfo::setIdentifier(const QByteArray &newIdentifier)
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
    d.space() << "mText" << t.text();
    d.space() << "mTitle" << t.title();
    d.space() << "mLink" << t.link();
    d.space() << "mRead" << t.read();
    d.space() << "mTextArguments" << t.textArguments();
    d.space() << "mIdentifier" << t.identifier();
    d.space() << "mPriority" << t.priority();
    return d;
}
