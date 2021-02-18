/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "retentioninfo.h"

RetentionInfo::RetentionInfo()
{
}

RetentionInfo::~RetentionInfo()
{
}

bool RetentionInfo::isNotDefault() const
{
    return mEnabled || mExcludePinned || mFilesOnly || mOverrideGlobal || (mMaxAge != -1);
}

void RetentionInfo::parseRetentionInfo(const QJsonObject &replyObject)
{
    qDebug() << " void RetentionInfo::parseRetentionInfo(const QJsonObject &replyObject)" << replyObject;
    mEnabled = replyObject.value(QLatin1String("enabled")).toBool(false);
    mExcludePinned = replyObject.value(QLatin1String("excludePinned")).toBool(false);
    mFilesOnly = replyObject.value(QLatin1String("filesOnly")).toBool(false);
    mOverrideGlobal = replyObject.value(QLatin1String("overrideGlobal")).toBool(false);
    mMaxAge = replyObject.value(QLatin1String("maxAge")).toInt(-1);
}

bool RetentionInfo::enabled() const
{
    return mEnabled;
}

void RetentionInfo::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

bool RetentionInfo::overrideGlobal() const
{
    return mOverrideGlobal;
}

void RetentionInfo::setOverrideGlobal(bool overrideGlobal)
{
    mOverrideGlobal = overrideGlobal;
}

bool RetentionInfo::excludePinned() const
{
    return mExcludePinned;
}

void RetentionInfo::setExcludePinned(bool excludePinned)
{
    mExcludePinned = excludePinned;
}

bool RetentionInfo::filesOnly() const
{
    return mFilesOnly;
}

void RetentionInfo::setFilesOnly(bool filesOnly)
{
    mFilesOnly = filesOnly;
}

int RetentionInfo::maxAge() const
{
    return mMaxAge;
}

void RetentionInfo::setMaxAge(int maxAge)
{
    mMaxAge = maxAge;
}

bool RetentionInfo::operator==(const RetentionInfo &other) const
{
    return mMaxAge == other.maxAge() && mEnabled == other.enabled() && mOverrideGlobal == other.overrideGlobal() && mExcludePinned == other.excludePinned()
        && mFilesOnly == other.filesOnly();
}

bool RetentionInfo::operator!=(const RetentionInfo &other) const
{
    return !RetentionInfo::operator==(other);
}

QJsonObject RetentionInfo::serialize(const RetentionInfo &retention)
{
    QJsonObject obj;
    obj[QStringLiteral("enabled")] = retention.enabled();
    obj[QStringLiteral("overrideGlobal")] = retention.overrideGlobal();
    obj[QStringLiteral("excludePinned")] = retention.excludePinned();
    obj[QStringLiteral("filesOnly")] = retention.filesOnly();
    obj[QStringLiteral("maxAge")] = retention.maxAge();
    return obj;
}

RetentionInfo RetentionInfo::fromJSon(const QJsonObject &obj)
{
    RetentionInfo info;
    info.setEnabled(obj[QStringLiteral("enabled")].toBool());
    info.setOverrideGlobal(obj[QStringLiteral("overrideGlobal")].toBool());
    info.setExcludePinned(obj[QStringLiteral("excludePinned")].toBool());
    info.setFilesOnly(obj[QStringLiteral("filesOnly")].toBool());
    info.setMaxAge(obj[QStringLiteral("maxAge")].toInt(-1));

    return info;
}

QDebug operator<<(QDebug d, const RetentionInfo &t)
{
    d << "Enabled " << t.enabled();
    d << "overrideGlobal " << t.overrideGlobal();
    d << "excludePinned " << t.excludePinned();
    d << "filesOnly " << t.filesOnly();
    d << "maxAge " << t.maxAge();
    return d;
}
