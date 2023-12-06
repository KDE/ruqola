/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ollamatextplugin.h"
#include "ollamatextinterface.h"
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(OllamaTextPlugin, "ruqola_ollamatextplugin.json")

OllamaTextPlugin::OllamaTextPlugin(QObject *parent, const QVariantList &)
    : PluginText(parent)
{
}

OllamaTextPlugin::~OllamaTextPlugin() = default;

PluginTextInterface *OllamaTextPlugin::createInterface(QObject *parent)
{
    return new OllamaTextInterface(parent);
}

#include "ollamatextplugin.moc"

#include "moc_ollamatextplugin.cpp"
