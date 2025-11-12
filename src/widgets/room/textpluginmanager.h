/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#if HAVE_PLUGIN_UTILS_SUPPORT
#include <TextAddonsWidgets/PluginUtil>
#else
#include "plugins/pluginutils.h"
#endif
#include <KPluginMetaData>
#include <QList>
#include <QObject>
class PluginText;

class TextPluginManagerInfo
{
public:
    TextPluginManagerInfo() = default;

    KPluginMetaData data;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
#if HAVE_PLUGIN_UTILS_SUPPORT
    TextAddonsWidgets::PluginUtilData pluginData;
#else
    PluginUtils::PluginUtilData pluginData;
#endif
    PluginText *plugin = nullptr;
    bool isEnabled = true;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextPluginManager : public QObject
{
    Q_OBJECT
public:
    ~TextPluginManager() override;
    static TextPluginManager *self();

    [[nodiscard]] QList<PluginText *> pluginsList() const;

    [[nodiscard]] QString configGroupName() const;
    [[nodiscard]] QString configPrefixSettingKey() const;

#if HAVE_PLUGIN_UTILS_SUPPORT
    [[nodiscard]] QList<TextAddonsWidgets::PluginUtilData> pluginDataList() const;
#else
    [[nodiscard]] QList<PluginUtils::PluginUtilData> pluginDataList() const;
#endif

    [[nodiscard]] PluginText *pluginFromIdentifier(const QString &identifier) const;

private:
    explicit TextPluginManager(QObject *parent = nullptr);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializePluginList();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadPlugin(TextPluginManagerInfo *item);
    QList<TextPluginManagerInfo> mPluginList;
#if HAVE_PLUGIN_UTILS_SUPPORT
    QList<TextAddonsWidgets::PluginUtilData> mPluginDataList;
#else
    QList<PluginUtils::PluginUtilData> mPluginDataList;
#endif
};
