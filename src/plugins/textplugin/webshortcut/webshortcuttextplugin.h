/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include "room/plugins/plugintext.h"

class WebShortcutTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit WebShortcutTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~WebShortcutTextPlugin() override;

    PluginTextInterface *createInterface(QObject *parent) override;
};

