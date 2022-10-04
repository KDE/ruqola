/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintext.h"
#include <QObject>
#include <QVariantList>

class TextToSpeechTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit TextToSpeechTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~TextToSpeechTextPlugin() override;

    PluginTextInterface *createInterface(QObject *parent) override;
};
