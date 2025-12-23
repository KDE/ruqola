/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintextinterface.h"
namespace TextAutoGenerateText
{
class TextAutoGenerateMenuWidget;
class TextAutoGenerateMenuTextManager;
}
class AiTextInterface : public PluginTextInterface
{
    Q_OBJECT
public:
    explicit AiTextInterface(TextAutoGenerateText::TextAutoGenerateMenuTextManager *manager, QWidget *parentWidget, QObject *parent = nullptr);
    ~AiTextInterface() override;

    void addAction(QMenu *menu) override;

    void setSelectedText(const QString &str) override;

private:
    TextAutoGenerateText::TextAutoGenerateMenuWidget *const mMenuWidget;
};
