/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room/plugins/plugintoolinterface.h"

class AutoGenerateTextPluginToolInterface : public PluginToolInterface
{
    Q_OBJECT
public:
    explicit AutoGenerateTextPluginToolInterface(QObject *parent = nullptr);
    ~AutoGenerateTextPluginToolInterface() override;
    void activateTool() override;
};
