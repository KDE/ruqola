/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#include <KPluginMetaData>
#include <QList>
#include <QObject>
#include <TextAddonsWidgets/PluginUtil>

class PluginTool;

class ToolsPluginManagerInfo
{
public:
    ToolsPluginManagerInfo() = default;

    KPluginMetaData data;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    TextAddonsWidgets::PluginUtilData pluginData;
    PluginTool *plugin = nullptr;
    bool isEnabled = true;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT ToolsPluginManager : public QObject
{
    Q_OBJECT
public:
    ~ToolsPluginManager() override;
    static ToolsPluginManager *self();

    [[nodiscard]] QList<PluginTool *> pluginsList() const;

    [[nodiscard]] QString configGroupName() const;
    [[nodiscard]] QString configPrefixSettingKey() const;

    [[nodiscard]] QList<TextAddonsWidgets::PluginUtilData> pluginDataList() const;
    [[nodiscard]] PluginTool *pluginFromIdentifier(const QString &identifier) const;

private:
    explicit ToolsPluginManager(QObject *parent = nullptr);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializePluginList();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadPlugin(ToolsPluginManagerInfo *item);
    QList<ToolsPluginManagerInfo> mPluginList;
    QList<TextAddonsWidgets::PluginUtilData> mPluginDataList;
};
