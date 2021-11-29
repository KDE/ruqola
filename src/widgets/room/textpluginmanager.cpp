/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "textpluginmanager.h"
#include "kcoreaddons_version.h"
#include "plugins/plugintext.h"

#include <KPluginFactory>
#include <KPluginLoader>
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
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 86, 0)
    const QVector<KPluginMetaData> plugins = KPluginLoader::findPlugins(QStringLiteral("ruqolaplugins/textplugins"));
#else
    const QVector<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("ruqolaplugins/textplugins"));
#endif

    QVectorIterator<KPluginMetaData> i(plugins);
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
    QVector<TextPluginManagerInfo>::iterator end(mPluginList.end());
    for (QVector<TextPluginManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
}

void TextPluginManager::loadPlugin(TextPluginManagerInfo *item)
{
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 86, 0)
    KPluginLoader pluginLoader(item->metaDataFileName);
    if (pluginLoader.factory()) {
        item->plugin = pluginLoader.factory()->create<PluginText>(this, QVariantList() << item->metaDataFileNameBaseName);
        mPluginDataList.append(item->pluginData);
    }
#else
    if (auto plugin = KPluginFactory::instantiatePlugin<PluginText>(item->data, this, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        mPluginDataList.append(item->pluginData);
    }
#endif
}

QVector<PluginText *> TextPluginManager::pluginsList() const
{
    QVector<PluginText *> lst;
    QVector<TextPluginManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<TextPluginManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
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
