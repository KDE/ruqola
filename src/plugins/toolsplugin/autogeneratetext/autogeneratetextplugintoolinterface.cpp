/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogeneratetextplugintoolinterface.h"

AutoGenerateTextPluginToolInterface::AutoGenerateTextPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

AutoGenerateTextPluginToolInterface::~AutoGenerateTextPluginToolInterface() = default;

void AutoGenerateTextPluginToolInterface::activateTool()
{
}
