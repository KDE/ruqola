/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenplugintoolinterface.h"

GrabScreenPluginToolInterface::GrabScreenPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

GrabScreenPluginToolInterface::~GrabScreenPluginToolInterface() = default;
