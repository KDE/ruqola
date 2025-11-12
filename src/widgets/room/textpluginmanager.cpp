/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textpluginmanager.h"

#include "plugins/plugintext.h"

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QFileInfo>
#include <QSet>

using namespace Qt::Literals::StringLiterals;
TextPluginManager::TextPluginManager(QObject *parent)
    : QObject(parent)
{
    initializePluginList();
}

TextPluginManager::~TextPluginManager() = default;

TextPluginManager *TextPluginManager::self()
{
    static TextPluginManager s_self;
    return &s_self;
}

QString TextPluginManager::configGroupName() const
{
    return u"RuqolaPlugin-textplugins"_s;
}

QString TextPluginManager::configPrefixSettingKey() const
{
    return u"textpluginsPlugin"_s;
}

void TextPluginManager::initializePluginList()
{
    const QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(u"ruqolaplugins/textplugins"_s);
#if HAVE_PLUGIN_UTILS_SUPPORT
    const TextAddonsWidgets::PluginUtil::PluginsStateList states =
        TextAddonsWidgets::PluginUtil::loadPluginSetting({}, configGroupName(), configPrefixSettingKey());
#else
    const PluginUtils::PluginsStateList states = PluginUtils::loadPluginSetting(configGroupName(), configPrefixSettingKey());
#endif

    QListIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        TextPluginManagerInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
#if HAVE_PLUGIN_UTILS_SUPPORT
        info.pluginData = TextAddonsWidgets::PluginUtil::createPluginMetaData(data);
#else
        info.pluginData = PluginUtils::createPluginMetaData(data);
#endif
        // 2) look at if plugin is activated
#if HAVE_PLUGIN_UTILS_SUPPORT
        const bool isPluginActivated = TextAddonsWidgets::PluginUtil::isPluginActivated(states.enabledPluginList,
                                                                                        states.disabledPluginList,
                                                                                        info.pluginData.mEnableByDefault,
                                                                                        info.pluginData.mIdentifier);
#else
        const bool isPluginActivated =
            PluginUtils::isPluginActivated(states.enabledPluginList, states.disabledPluginList, info.pluginData.mEnableByDefault, info.pluginData.mIdentifier);
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
    QList<TextPluginManagerInfo>::iterator end(mPluginList.end());
    for (QList<TextPluginManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
}

void TextPluginManager::loadPlugin(TextPluginManagerInfo *item)
{
    if (auto plugin = KPluginFactory::instantiatePlugin<PluginText>(item->data, this, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        item->plugin->setEnabled(item->isEnabled);
        item->pluginData.mHasConfigureDialog = item->plugin->hasConfigureDialog();
        mPluginDataList.append(item->pluginData);
    }
}

#if HAVE_PLUGIN_UTILS_SUPPORT
QList<TextAddonsWidgets::PluginUtilData> TextPluginManager::pluginDataList() const
#else
QList<PluginUtils::PluginUtilData> TextPluginManager::pluginDataList() const
#endif
{
    return mPluginDataList;
}

PluginText *TextPluginManager::pluginFromIdentifier(const QString &identifier) const
{
    const auto it = std::find_if(mPluginList.constBegin(), mPluginList.constEnd(), [identifier](const TextPluginManagerInfo &info) {
        return info.pluginData.mIdentifier == identifier;
    });
    if (it != mPluginList.end()) {
        return (*it).plugin;
    }
    return nullptr;
}

QList<PluginText *> TextPluginManager::pluginsList() const
{
    QList<PluginText *> lst;
    lst.reserve(mPluginList.count());
    QList<TextPluginManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<TextPluginManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

#include "moc_textpluginmanager.cpp"
