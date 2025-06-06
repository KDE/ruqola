/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintextinterface.h"
#include "textautogeneratetext_version.h"
namespace TextAutoGenerateText
{
class TextAutoGenerateMenuWidget;
class TextAutoGenerateMenuTextManager;
}
class AiTextInterface : public PluginTextInterface
{
    Q_OBJECT
public:
#if TEXTAUTOGENERATETEXT_VERSION >= QT_VERSION_CHECK(1, 6, 1)
    explicit AiTextInterface(TextAutoGenerateText::TextAutoGenerateMenuTextManager *manager, QObject *parent = nullptr);
#else
    explicit AiTextInterface(QObject *parent = nullptr);
#endif
    ~AiTextInterface() override;

    void addAction(QMenu *menu) override;

    void setSelectedText(const QString &str) override;

private:
    TextAutoGenerateText::TextAutoGenerateMenuWidget *const mMenuWidget;
};
