/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pluginutils.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqolawidgets_debug.h"
#include <KConfigGroup>
#include <KSharedConfig>

PluginUtils::PluginUtilData PluginUtils::createPluginMetaData(const KPluginMetaData &metaData)
{
    const PluginUtils::PluginUtilData pluginData{
        .mIdentifier = metaData.pluginId(),
        .mName = metaData.name(),
        .mDescription = metaData.description(),
        .mEnableByDefault = metaData.isEnabledByDefault(),
    };
    if (pluginData.mName.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Plugin name not define. It's a bug:" << metaData;
    }
    return pluginData;
}

bool PluginUtils::isPluginActivated(const QStringList &enabledPluginsList,
                                    const QStringList &disabledPluginsList,
                                    bool isEnabledByDefault,
                                    const QString &pluginId)
{
    if (pluginId.isEmpty()) {
        return false;
    }
    const bool pluginEnabledByUser = enabledPluginsList.contains(pluginId);
    const bool pluginDisabledByUser = disabledPluginsList.contains(pluginId);
    if ((isEnabledByDefault && !pluginDisabledByUser) || (!isEnabledByDefault && pluginEnabledByUser)) {
        return true;
    }
    return false;
}

QPair<QStringList, QStringList> PluginUtils::loadPluginSetting(const QString &groupName, const QString &prefixSettingKey)
{
    QPair<QStringList, QStringList> pair;
    KSharedConfigPtr config = KSharedConfig::openConfig();
    QStringList enabledPlugins;
    QStringList disabledPlugins;
    if (config->hasGroup(groupName)) {
        const KConfigGroup grp = config->group(groupName);
        enabledPlugins = grp.readEntry(u"%1Enabled"_s.arg(prefixSettingKey), QStringList());
        disabledPlugins = grp.readEntry(u"%1Disabled"_s.arg(prefixSettingKey), QStringList());
    }

    pair.first = enabledPlugins;
    pair.second = disabledPlugins;
    return pair;
}

void PluginUtils::savePluginSettings(const QString &groupName,
                                     const QString &prefixSettingKey,
                                     const QStringList &enabledPluginsList,
                                     const QStringList &disabledPluginsList)
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup grp = config->group(groupName);
    if (enabledPluginsList.isEmpty()) {
        grp.deleteEntry(u"%1Enabled"_s.arg(prefixSettingKey));
    } else {
        grp.writeEntry(u"%1Enabled"_s.arg(prefixSettingKey), enabledPluginsList);
    }
    if (disabledPluginsList.isEmpty()) {
        grp.deleteEntry(u"%1Disabled"_s.arg(prefixSettingKey));
    } else {
        grp.writeEntry(u"%1Disabled"_s.arg(prefixSettingKey), disabledPluginsList);
    }
}
