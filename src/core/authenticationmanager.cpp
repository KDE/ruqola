/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "authenticationmanager.h"
#include "kcoreaddons_version.h"
#include "ruqola_debug.h"
#include <KPluginFactory>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <QFileInfo>

#include <plugins/pluginauthentication.h>

AuthenticationManager::AuthenticationManager(QObject *parent)
    : QObject(parent)
{
    initializePluginList();
}

AuthenticationManager::~AuthenticationManager() = default;

AuthenticationManager *AuthenticationManager::self()
{
    static AuthenticationManager s_self;
    return &s_self;
}

void AuthenticationManager::initializePluginList()
{
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 86, 0)
    const QVector<KPluginMetaData> plugins = KPluginLoader::findPlugins(QStringLiteral("ruqolaplugins/authentication"));
#else
    const QVector<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("ruqolaplugins/authentication"));
#endif

    QVectorIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        AuthenticationManagerInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
        info.pluginData = createPluginMetaData(data);
        // 2) look at if plugin is activated
        info.data = data;
        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.metaDataFileName = data.fileName();
        // only load plugins once, even if found multiple times!
        if (unique.contains(info.metaDataFileNameBaseName)) {
            continue;
        }
        info.plugin = nullptr;
        mPluginList.push_back(info);
        unique.insert(info.metaDataFileNameBaseName);
    }
    QVector<AuthenticationManagerInfo>::iterator end(mPluginList.end());
    for (QVector<AuthenticationManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
}

void AuthenticationManager::loadPlugin(AuthenticationManagerInfo *item)
{
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 86, 0)
    KPluginLoader pluginLoader(item->metaDataFileName);
    if (pluginLoader.factory()) {
        item->plugin = pluginLoader.factory()->create<PluginAuthentication>(this, QVariantList() << item->metaDataFileNameBaseName);
        mPluginDataList.append(item->pluginData);
    }
#else
    if (auto plugin = KPluginFactory::instantiatePlugin<PluginAuthentication>(item->data, this, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        mPluginDataList.append(item->pluginData);
    }
#endif
}

QVector<PluginAuthentication *> AuthenticationManager::pluginsList() const
{
    QVector<PluginAuthentication *> lst;
    QVector<AuthenticationManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<AuthenticationManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

PluginUtilData AuthenticationManager::createPluginMetaData(const KPluginMetaData &metaData)
{
    PluginUtilData pluginData;
    pluginData.mName = metaData.name();
    pluginData.mIdentifier = metaData.pluginId();
    return pluginData;
}
