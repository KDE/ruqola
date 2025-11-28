/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "room/plugins/plugintool.h"
#include <QVariant>

class AiActionsPluginTool : public PluginTool
{
    Q_OBJECT
public:
    explicit AiActionsPluginTool(QObject *parent = nullptr, const QVariantList & = {});
    ~AiActionsPluginTool() override;

    [[nodiscard]] bool toolFound() const override;
    [[nodiscard]] ToolType toolType() const override;
    [[nodiscard]] QString iconName() const override;
    [[nodiscard]] QString toolTip() const override;
    [[nodiscard]] QString description() const override;
    [[nodiscard]] PluginToolInterface *createInterface(QWidget *parentWidget, QObject *parent) override;
    [[nodiscard]] int order() const override;
    [[nodiscard]] bool hasMenu() const override;
    [[nodiscard]] bool hasConfigureDialog() const override;
};
