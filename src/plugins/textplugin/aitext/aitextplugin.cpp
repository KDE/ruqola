/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextplugin.h"
#include "aitextinterface.h"
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(AiTextPlugin, "ruqola_aitextplugin.json")

AiTextPlugin::AiTextPlugin(QObject *parent, const QVariantList &)
    : PluginText(parent)
{
}

AiTextPlugin::~AiTextPlugin() = default;

PluginTextInterface *AiTextPlugin::createInterface(QObject *parent)
{
    return new AiTextInterface(parent);
}

int AiTextPlugin::order() const
{
    return 30;
}

#include "aitextplugin.moc"

#include "moc_aitextplugin.cpp"
