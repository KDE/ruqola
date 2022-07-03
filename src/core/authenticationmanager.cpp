/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanager.h"
#include "ruqola_debug.h"
#include <KPluginFactory>
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
    const QVector<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("ruqolaplugins/authentication"));

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
    if (auto plugin = KPluginFactory::instantiatePlugin<PluginAuthentication>(item->data, this, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        mPluginDataList.append(item->pluginData);
    }
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
