/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintext.h"
#include <QVariant>
namespace TextAutoGenerateText
{
class TextAutoGenerateMenuTextManager;
}
class AiTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit AiTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~AiTextPlugin() override;

    [[nodiscard]] PluginTextInterface *createInterface(QWidget *parentWidget, QObject *parent) override;
    [[nodiscard]] int order() const override;

    [[nodiscard]] bool hasConfigureDialog() const override;
    void showConfigureDialog(QWidget *parent = nullptr) const override;

private:
    TextAutoGenerateText::TextAutoGenerateMenuTextManager *const mManager;
};
