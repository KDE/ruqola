/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sharetextplugin.h"
#include "sharetextinterface.h"
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(ShareTextPlugin, "ruqola_sharetextplugin.json")

ShareTextPlugin::ShareTextPlugin(QObject *parent, const QVariantList &)
    : PluginText(parent)
{
}

ShareTextPlugin::~ShareTextPlugin() = default;

PluginTextInterface *ShareTextPlugin::createInterface(QObject *parent)
{
    return new ShareTextInterface(parent);
}

#include "sharetextplugin.moc"
