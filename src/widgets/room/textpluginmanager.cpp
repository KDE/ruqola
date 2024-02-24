/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textpluginmanager.h"
#include "plugins/plugintext.h"

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QFileInfo>
#include <QSet>

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

void TextPluginManager::initializePluginList()
{
    const QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("ruqolaplugins/textplugins"));

    QListIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        TextPluginManagerInfo info;
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
    QList<TextPluginManagerInfo>::iterator end(mPluginList.end());
    for (QList<TextPluginManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
}

void TextPluginManager::loadPlugin(TextPluginManagerInfo *item)
{
    if (auto plugin = KPluginFactory::instantiatePlugin<PluginText>(item->data, this, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        mPluginDataList.append(item->pluginData);
    }
}

QList<PluginText *> TextPluginManager::pluginsList() const
{
    QList<PluginText *> lst;
    QList<TextPluginManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<TextPluginManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

PluginTextUtilData TextPluginManager::createPluginMetaData(const KPluginMetaData &metaData)
{
    PluginTextUtilData pluginData;
    pluginData.mName = metaData.name();
    pluginData.mIdentifier = metaData.pluginId();
    return pluginData;
}

#include "moc_textpluginmanager.cpp"
