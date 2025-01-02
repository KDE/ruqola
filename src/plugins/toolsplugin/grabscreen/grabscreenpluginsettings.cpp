/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenpluginsettings.h"

GrabScreenPluginSettings::GrabScreenPluginSettings() = default;

GrabScreenPluginSettings::~GrabScreenPluginSettings() = default;

int GrabScreenPluginSettings::delay() const
{
    return mDelay;
}

void GrabScreenPluginSettings::setDelay(int newDelay)
{
    mDelay = newDelay;
}
