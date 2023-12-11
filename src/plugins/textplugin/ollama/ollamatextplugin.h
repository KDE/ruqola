/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintext.h"
class QObject;
#include <QVariant>

class OllamaTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit OllamaTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~OllamaTextPlugin() override;

    [[nodiscard]] PluginTextInterface *createInterface(QObject *parent) override;
};
