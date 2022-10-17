/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QVariant>

#include "room/plugins/plugintext.h"

class TextToSpeechTextPlugin : public PluginText
{
    Q_OBJECT
public:
    explicit TextToSpeechTextPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~TextToSpeechTextPlugin() override;

    PluginTextInterface *createInterface(QObject *parent) override;
};
