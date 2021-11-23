/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
