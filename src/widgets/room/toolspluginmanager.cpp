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

using namespace Qt::Literals::StringLiterals;
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

QString ToolsPluginManager::configGroupName() const
{
    return u"RuqolaPlugin-toolsplugins"_s;
}

QString ToolsPluginManager::configPrefixSettingKey() const
{
    return u"toolspluginsPlugin"_s;
}

void ToolsPluginManager::initializePluginList()
{
    const QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(u"ruqolaplugins/toolsplugins"_s);
#if HAVE_PLUGIN_UTILS_SUPPORT
    const TextAddonsWidgets::PluginUtil::PluginsStateList pair =
        TextAddonsWidgets::PluginUtil::loadPluginSetting({}, configGroupName(), configPrefixSettingKey());
#else
    const PluginUtils::PluginsStateList pair = PluginUtils::loadPluginSetting(configGroupName(), configPrefixSettingKey());
#endif
    QListIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        ToolsPluginManagerInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
#if HAVE_PLUGIN_UTILS_SUPPORT
        info.pluginData = TextAddonsWidgets::PluginUtil::createPluginMetaData(data);
#else
        info.pluginData = PluginUtils::createPluginMetaData(data);
#endif
        // 2) look at if plugin is activated
#if HAVE_PLUGIN_UTILS_SUPPORT
        const bool isPluginActivated = TextAddonsWidgets::PluginUtil::isPluginActivated(pair.enabledPluginList,
                                                                                        pair.disabledPluginList,
                                                                                        info.pluginData.mEnableByDefault,
                                                                                        info.pluginData.mIdentifier);
#else
        const bool isPluginActivated =
            PluginUtils::isPluginActivated(pair.enabledPluginList, pair.disabledPluginList, info.pluginData.mEnableByDefault, info.pluginData.mIdentifier);
#endif
        info.isEnabled = isPluginActivated;
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
        item->plugin->setEnabled(item->isEnabled);
        item->pluginData.mHasConfigureDialog = item->plugin->hasConfigureDialog();
        mPluginDataList.append(item->pluginData);
    }
}

PluginTool *ToolsPluginManager::pluginFromIdentifier(const QString &identifier) const
{
    const auto it = std::find_if(mPluginList.constBegin(), mPluginList.constEnd(), [identifier](const ToolsPluginManagerInfo &info) {
        return info.pluginData.mIdentifier == identifier;
    });
    if (it != mPluginList.end()) {
        return (*it).plugin;
    }
    return nullptr;
}

#if HAVE_PLUGIN_UTILS_SUPPORT
QList<TextAddonsWidgets::PluginUtilData> ToolsPluginManager::pluginDataList() const
#else
QList<PluginUtils::PluginUtilData> ToolsPluginManager::pluginDataList() const
#endif
{
    return mPluginDataList;
}

QList<PluginTool *> ToolsPluginManager::pluginsList() const
{
    QList<PluginTool *> lst;
    lst.reserve(mPluginList.count());
    QList<ToolsPluginManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<ToolsPluginManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

#include "moc_toolspluginmanager.cpp"
