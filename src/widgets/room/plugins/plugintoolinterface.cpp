/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintoolinterface.h"

PluginToolInterface::PluginToolInterface(QObject *parent)
    : QObject{parent}
{
}

PluginToolInterface::~PluginToolInterface() = default;

void PluginToolInterface::activateTool()
{
    // Nothing here. Reimplement it
}

#include "moc_plugintoolinterface.cpp"
