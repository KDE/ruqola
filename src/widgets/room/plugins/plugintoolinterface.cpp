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

#include "moc_plugintoolinterface.cpp"
