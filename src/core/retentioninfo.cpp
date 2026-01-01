/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentioninfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(RetentionInfo, Ruqola_RetentionInfo)

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
RetentionInfo::RetentionInfo() = default;

bool RetentionInfo::isNotDefault() const
{
    return mEnabled || mExcludePinned || mFilesOnly || mOverrideGlobal || (mMaxAge != -1);
}

void RetentionInfo::parseRetentionInfo(const QJsonObject &replyObject)
{
    mEnabled = replyObject.value("enabled"_L1).toBool(false);
    mExcludePinned = replyObject.value("excludePinned"_L1).toBool(false);
    mFilesOnly = replyObject.value("filesOnly"_L1).toBool(false);
    mOverrideGlobal = replyObject.value("overrideGlobal"_L1).toBool(false);
    mMaxAge = replyObject.value("maxAge"_L1).toInt(-1);
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
    obj["enabled"_L1] = retention.enabled();
    obj["overrideGlobal"_L1] = retention.overrideGlobal();
    obj["excludePinned"_L1] = retention.excludePinned();
    obj["filesOnly"_L1] = retention.filesOnly();
    obj["maxAge"_L1] = retention.maxAge();
    return obj;
}

RetentionInfo RetentionInfo::deserialize(const QJsonObject &obj)
{
    RetentionInfo info;
    info.setEnabled(obj["enabled"_L1].toBool());
    info.setOverrideGlobal(obj["overrideGlobal"_L1].toBool());
    info.setExcludePinned(obj["excludePinned"_L1].toBool());
    info.setFilesOnly(obj["filesOnly"_L1].toBool());
    info.setMaxAge(obj["maxAge"_L1].toInt(-1));

    return info;
}

QDebug operator<<(QDebug d, RetentionInfo t)
{
    d.space() << "Enabled" << t.enabled();
    d.space() << "overrideGlobal" << t.overrideGlobal();
    d.space() << "excludePinned" << t.excludePinned();
    d.space() << "filesOnly" << t.filesOnly();
    d.space() << "maxAge" << t.maxAge();
    return d;
}
