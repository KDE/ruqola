/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "activitiesmanager.h"

ActivitiesManager::ActivitiesManager(QObject *parent)
    : QObject{parent}
{
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
