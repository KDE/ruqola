/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintext.h"
#include <QVariant>

class WebShortcutTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit WebShortcutTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~WebShortcutTextPlugin() override;

    [[nodiscard]] PluginTextInterface *createInterface(QObject *parent) override;

    [[nodiscard]] int order() const override;
};
