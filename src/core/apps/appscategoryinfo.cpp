/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscategoryinfo.h"
#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(AppsCategoryInfo, Ruqola_AppsCategoryInfo)

using namespace Qt::Literals::StringLiterals;
AppsCategoryInfo::AppsCategoryInfo() = default;

AppsCategoryInfo::~AppsCategoryInfo() = default;

void AppsCategoryInfo::parseAppsCategoryInfo(const QJsonObject &replyObject)
{
    mTitle = replyObject["title"_L1].toString();
    mIdentifier = replyObject["id"_L1].toString().toLatin1();
    mHidden = replyObject["hidden"_L1].toBool(false);
}

QString AppsCategoryInfo::title() const
{
    return mTitle;
}

void AppsCategoryInfo::setTitle(const QString &newTitle)
{
    mTitle = newTitle;
}

bool AppsCategoryInfo::hidden() const
{
    return mHidden;
}

void AppsCategoryInfo::setHidden(bool newHidden)
{
    mHidden = newHidden;
}

QByteArray AppsCategoryInfo::identifier() const
{
    return mIdentifier;
}

void AppsCategoryInfo::setIdentifier(const QByteArray &newIdentifier)
{
    mIdentifier = newIdentifier;
}

bool AppsCategoryInfo::isValid() const
{
    return !mIdentifier.isEmpty();
}

QDebug operator<<(QDebug d, const AppsCategoryInfo &t)
{
    d.space() << "mTitle" << t.title();
    d.space() << "mHidden" << t.hidden();
    d.space() << "mIdentifier" << t.identifier();
    return d;
}
