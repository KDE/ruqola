/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintext.h"
class QObject;
#include <QVariant>

class ShareTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit ShareTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~ShareTextPlugin() override;

    [[nodiscard]] PluginTextInterface *createInterface(QObject *parent) override;
    [[nodiscard]] int order() const override;
};
