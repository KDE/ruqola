/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KPluginMetaData>
#include <QList>
#include <QObject>

class PluginTool;

class PluginToolsUtilData
{
public:
    PluginToolsUtilData() = default;

    QStringList mExtraInfo;
    QString mIdentifier;
    QString mName;
};

class ToolsPluginManagerInfo
{
public:
    ToolsPluginManagerInfo() = default;

    KPluginMetaData data;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    PluginToolsUtilData pluginData;
    PluginTool *plugin = nullptr;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT ToolsPluginManager : public QObject
{
    Q_OBJECT
public:
    ~ToolsPluginManager() override;
    static ToolsPluginManager *self();

    [[nodiscard]] QList<PluginTool *> pluginsList() const;

private:
    explicit ToolsPluginManager(QObject *parent = nullptr);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializePluginList();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadPlugin(ToolsPluginManagerInfo *item);
    LIBRUQOLAWIDGETS_NO_EXPORT PluginToolsUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QList<ToolsPluginManagerInfo> mPluginList;
    QList<PluginToolsUtilData> mPluginDataList;
};
