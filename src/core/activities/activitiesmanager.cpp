/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "activitiesmanager.h"
#include "config-ruqola.h"
#include "ruqola_plasma_activities_debug.h"
#include "ruqolaglobalconfig.h"
#include <PlasmaActivities/Consumer>
ActivitiesManager::ActivitiesManager(QObject *parent)
    : QObject{parent}
    , mActivitiesConsumer(new KActivities::Consumer(this))
{
    mEnabled = RuqolaGlobalConfig::self()->plasmaActivities();
    connect(mActivitiesConsumer, &KActivities::Consumer::currentActivityChanged, this, [this](const QString &activityId) {
        qCDebug(RUQOLA_PLASMAACTIVITIES_LOG) << " switch to activity " << activityId;
        Q_EMIT activitiesChanged();
    });
    connect(mActivitiesConsumer, &KActivities::Consumer::serviceStatusChanged, this, &ActivitiesManager::activitiesChanged);
    if (mActivitiesConsumer->serviceStatus() != KActivities::Consumer::ServiceStatus::Running) {
        qCWarning(RUQOLA_PLASMAACTIVITIES_LOG) << "Plasma activities is not running: " << mActivitiesConsumer->serviceStatus();
    }
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

bool ActivitiesManager::isInCurrentActivity(const QStringList &lst) const
{
    if (mActivitiesConsumer->serviceStatus() == KActivities::Consumer::ServiceStatus::Running) {
        return lst.contains(mActivitiesConsumer->currentActivity());
    } else {
        return true;
    }
}

#include "moc_activitiesmanager.cpp"
