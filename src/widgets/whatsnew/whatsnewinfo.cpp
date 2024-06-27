/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

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
