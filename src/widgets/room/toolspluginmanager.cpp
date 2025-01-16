/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "toolspluginmanager.h"
#include "plugins/plugintool.h"

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QFileInfo>
#include <QSet>

ToolsPluginManager::ToolsPluginManager(QObject *parent)
    : QObject(parent)
{
    initializePluginList();
}

ToolsPluginManager::~ToolsPluginManager() = default;

ToolsPluginManager *ToolsPluginManager::self()
{
    static ToolsPluginManager s_self;
    return &s_self;
}

void ToolsPluginManager::initializePluginList()
{
    const QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("ruqolaplugins/toolsplugins"));

    QListIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        ToolsPluginManagerInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
        info.pluginData = createPluginMetaData(data);
        // 2) look at if plugin is activated
        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.metaDataFileName = data.fileName();
        info.data = data;
        // only load plugins once, even if found multiple times!
        if (unique.contains(info.metaDataFileNameBaseName)) {
            continue;
        }
        info.plugin = nullptr;
        mPluginList.push_back(info);
        unique.insert(info.metaDataFileNameBaseName);
    }
    QList<ToolsPluginManagerInfo>::iterator end(mPluginList.end());
    for (QList<ToolsPluginManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
}

void ToolsPluginManager::loadPlugin(ToolsPluginManagerInfo *item)
{
    if (auto plugin = KPluginFactory::instantiatePlugin<PluginTool>(item->data, this, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        mPluginDataList.append(item->pluginData);
    }
}

QList<PluginTool *> ToolsPluginManager::pluginsList() const
{
    QList<PluginTool *> lst;
    QList<ToolsPluginManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<ToolsPluginManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

PluginToolsUtilData ToolsPluginManager::createPluginMetaData(const KPluginMetaData &metaData)
{
    const PluginToolsUtilData pluginData{
        .mExtraInfo = {},
        .mIdentifier = metaData.pluginId(),
        .mName = metaData.name(),
    };
    return pluginData;
}

#include "moc_toolspluginmanager.cpp"
