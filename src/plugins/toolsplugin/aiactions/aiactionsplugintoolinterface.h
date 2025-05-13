/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room/plugins/plugintoolinterface.h"

class AiActionsPluginToolInterface : public PluginToolInterface
{
public:
    explicit AiActionsPluginToolInterface(QObject *parent = nullptr);
    ~AiActionsPluginToolInterface() override;
    void activateTool() override;
};
