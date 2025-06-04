/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pluginutils.h"

PluginUtils::PluginUtilData PluginUtils::createPluginMetaData(const KPluginMetaData &metaData)
{
    const PluginUtils::PluginUtilData pluginData{
        .mIdentifier = metaData.pluginId(),
        .mName = metaData.name(),
        .mIsEnabled = metaData.isEnabledByDefault(),
    };
    return pluginData;
}
