/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_export.h"
#include <QObject>

class LIBRUQOLAWIDGETS_EXPORT PluginTool : public QObject
{
    Q_OBJECT
public:
    enum class ToolType : uint8_t {
        Tools,
        Channels,
        MessageViewToolBar,
    };
    explicit PluginTool(QObject *parent = nullptr);
    ~PluginTool() override;

    [[nodiscard]] virtual bool toolFound() const = 0;
    [[nodiscard]] virtual ToolType toolType() const = 0;
    [[nodiscard]] virtual QString iconName() const = 0;
    [[nodiscard]] virtual QString toolTip() const = 0;
    [[nodiscard]] virtual QString description() const = 0;

Q_SIGNALS:
    void activated();
};
