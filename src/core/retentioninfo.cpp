/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentioninfo.h"

RetentionInfo::RetentionInfo() = default;

bool RetentionInfo::isNotDefault() const
{
    return mEnabled || mExcludePinned || mFilesOnly || mOverrideGlobal || (mMaxAge != -1);
}

void RetentionInfo::parseRetentionInfo(const QJsonObject &replyObject)
{
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

QDebug operator<<(QDebug d, RetentionInfo t)
{
    d << "Enabled " << t.enabled();
    d << "overrideGlobal " << t.overrideGlobal();
    d << "excludePinned " << t.excludePinned();
    d << "filesOnly " << t.filesOnly();
    d << "maxAge " << t.maxAge();
    return d;
}
