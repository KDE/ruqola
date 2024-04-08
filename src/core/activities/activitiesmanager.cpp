/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "activitiesmanager.h"
#include "config-ruqola.h"
#include "ruqolaglobalconfig.h"
#include <PlasmaActivities/Consumer>
ActivitiesManager::ActivitiesManager(QObject *parent)
    : QObject{parent}
    , mActivitiesConsumer(new KActivities::Consumer(this))
{
#if HAS_ACTIVITY_SUPPORT
    mEnabled = RuqolaGlobalConfig::self()->plasmaActivities();
#endif
    connect(mActivitiesConsumer, &KActivities::Consumer::currentActivityChanged, this, &ActivitiesManager::activitiesChanged);
    connect(mActivitiesConsumer, &KActivities::Consumer::serviceStatusChanged, this, &ActivitiesManager::activitiesChanged);
}

ActivitiesManager::~ActivitiesManager() = default;

bool ActivitiesManager::enabled() const
{
    return mEnabled;
}

void ActivitiesManager::setEnabled(bool newEnabled)
{
    mEnabled = newEnabled;
}

#include "moc_activitiesmanager.cpp"
