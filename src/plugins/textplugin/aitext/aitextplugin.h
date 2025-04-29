/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintext.h"
#include <QVariant>

class AiTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit AiTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~AiTextPlugin() override;

    [[nodiscard]] PluginTextInterface *createInterface(QObject *parent) override;
    [[nodiscard]] int order() const override;
};
