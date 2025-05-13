/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aiactionsplugintoolinterface.h"

AiActionsPluginToolInterface::AiActionsPluginToolInterface(QObject *parent)
    : PluginToolInterface{parent}
{
}

AiActionsPluginToolInterface::~AiActionsPluginToolInterface() = default;

void AiActionsPluginToolInterface::activateTool()
{
}
