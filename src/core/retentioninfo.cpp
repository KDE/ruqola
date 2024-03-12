/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentioninfo.h"
#include <QJsonObject>

RetentionInfo::RetentionInfo() = default;

bool RetentionInfo::isNotDefault() const
{
    return mEnabled || mExcludePinned || mFilesOnly || mOverrideGlobal || (mMaxAge != -1);
}

void RetentionInfo::parseRetentionInfo(const QJsonObject &replyObject)
{
    mEnabled = replyObject.value(QLatin1StringView("enabled")).toBool(false);
    mExcludePinned = replyObject.value(QLatin1StringView("excludePinned")).toBool(false);
    mFilesOnly = replyObject.value(QLatin1StringView("filesOnly")).toBool(false);
    mOverrideGlobal = replyObject.value(QLatin1StringView("overrideGlobal")).toBool(false);
    mMaxAge = replyObject.value(QLatin1StringView("maxAge")).toInt(-1);
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

bool RetentionInfo::operator==(RetentionInfo other) const
{
    return mMaxAge == other.maxAge() && mEnabled == other.enabled() && mOverrideGlobal == other.overrideGlobal() && mExcludePinned == other.excludePinned()
        && mFilesOnly == other.filesOnly();
}

bool RetentionInfo::operator!=(RetentionInfo other) const
{
    return !RetentionInfo::operator==(other);
}

QJsonObject RetentionInfo::serialize(RetentionInfo retention)
{
    QJsonObject obj;
    obj[QLatin1StringView("enabled")] = retention.enabled();
    obj[QLatin1StringView("overrideGlobal")] = retention.overrideGlobal();
    obj[QLatin1StringView("excludePinned")] = retention.excludePinned();
    obj[QLatin1StringView("filesOnly")] = retention.filesOnly();
    obj[QLatin1StringView("maxAge")] = retention.maxAge();
    return obj;
}

RetentionInfo RetentionInfo::deserialize(const QJsonObject &obj)
{
    RetentionInfo info;
    info.setEnabled(obj[QLatin1StringView("enabled")].toBool());
    info.setOverrideGlobal(obj[QLatin1StringView("overrideGlobal")].toBool());
    info.setExcludePinned(obj[QLatin1StringView("excludePinned")].toBool());
    info.setFilesOnly(obj[QLatin1StringView("filesOnly")].toBool());
    info.setMaxAge(obj[QLatin1StringView("maxAge")].toInt(-1));

    return info;
}

QDebug operator<<(QDebug d, RetentionInfo t)
{
    d << "Enabled " << t.enabled();
    d << "overrideGlobal " << t.overrideGlobal();
    d << "excludePinned " << t.excludePinned();
    d << "filesOnly " << t.filesOnly();
    d << "maxAge " << t.maxAge();
    return d;
}
