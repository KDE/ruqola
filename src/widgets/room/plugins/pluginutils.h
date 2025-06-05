/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <KPluginMetaData>
#include <QString>

namespace PluginUtils
{
struct PluginUtilData {
    QString mIdentifier;
    QString mName;
    QString mDescription;
    bool mEnableByDefault = false;
};

[[nodiscard]] PluginUtils::PluginUtilData createPluginMetaData(const KPluginMetaData &metaData);
[[nodiscard]] bool
isPluginActivated(const QStringList &enabledPluginsList, const QStringList &disabledPluginsList, bool isEnabledByDefault, const QString &pluginId);
[[nodiscard]] QPair<QStringList, QStringList> loadPluginSetting(const QString &groupName, const QString &prefixSettingKey);
void savePluginSettings(const QString &groupName,
                        const QString &prefixSettingKey,
                        const QStringList &enabledPluginsList,
                        const QStringList &disabledPluginsList);
};
