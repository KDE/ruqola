/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintoolinterface.h"

class GrabScreenPluginToolInterface : public PluginToolInterface
{
public:
    explicit GrabScreenPluginToolInterface(QObject *parent = nullptr);
    ~GrabScreenPluginToolInterface() override;
    void activateTool() override;
};
