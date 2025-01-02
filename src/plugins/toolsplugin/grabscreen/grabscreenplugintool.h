/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "room/plugins/plugintool.h"

class GrabScreenPluginTool : public PluginTool
{
public:
    explicit GrabScreenPluginTool(QObject *parent = nullptr);
    ~GrabScreenPluginTool() override;

    [[nodiscard]] bool toolFound() const override;
    [[nodiscard]] ToolType toolType() const override;
    [[nodiscard]] QString iconName() const override;
    [[nodiscard]] QString toolTip() const override;
    [[nodiscard]] QString description() const override;
    [[nodiscard]] PluginToolInterface *createInterface(QObject *parent) override;
};
