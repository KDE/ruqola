/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewinfo.h"

WhatsNewInfo::WhatsNewInfo() = default;

WhatsNewInfo::~WhatsNewInfo() = default;

QStringList WhatsNewInfo::changes() const
{
    return mChanges;
}

void WhatsNewInfo::setChanges(const QStringList &newChanges)
{
    mChanges = newChanges;
}

QStringList WhatsNewInfo::newFeatures() const
{
    return mNewFeatures;
}

void WhatsNewInfo::setNewFeatures(const QStringList &newNewFeatures)
{
    mNewFeatures = newNewFeatures;
}

QStringList WhatsNewInfo::bugFixings() const
{
    return mBugFixings;
}

void WhatsNewInfo::setBugFixings(const QStringList &newBugFixings)
{
    mBugFixings = newBugFixings;
}

QString WhatsNewInfo::version() const
{
    return mVersion;
}

void WhatsNewInfo::setVersion(const QString &newVersion)
{
    mVersion = newVersion;
}

QDebug operator<<(QDebug d, const WhatsNewInfo &t)
{
    d.space() << "version" << t.version();
    d.space() << "bugFixings" << t.bugFixings();
    d.space() << "newFeatures" << t.newFeatures();
    d.space() << "changes" << t.changes();
    return d;
}
