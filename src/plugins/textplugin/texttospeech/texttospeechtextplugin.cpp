/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechtextplugin.h"
#include "texttospeechtextinterface.h"
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(TextToSpeechTextPlugin, "ruqola_texttospeechtextplugin.json")

TextToSpeechTextPlugin::TextToSpeechTextPlugin(QObject *parent, const QVariantList &)
    : PluginText(parent)
{
}

TextToSpeechTextPlugin::~TextToSpeechTextPlugin() = default;

PluginTextInterface *TextToSpeechTextPlugin::createInterface(QObject *parent)
{
    return new TextToSpeechTextInterface(parent);
}

#include "texttospeechtextplugin.moc"
