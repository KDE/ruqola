/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <KPluginMetaData>
#include <QList>
#include <QObject>
class PluginAuthentication;
class PluginUtilData
{
public:
    PluginUtilData() = default;

    QStringList mExtraInfo;
    QString mIdentifier;
    QString mName;
};

class AuthenticationManagerInfo
{
public:
    AuthenticationManagerInfo() = default;

    KPluginMetaData data;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    PluginUtilData pluginData;
    PluginAuthentication *plugin = nullptr;
};

class LIBRUQOLACORE_EXPORT AuthenticationManager : public QObject
{
    Q_OBJECT
public:
    enum AuthMethodType {
        Unknown = 0,
        Twitter = 1,
        FaceBook = 2,
        GitHub = 4,
        GitLab = 8,
        Google = 16,
        Linkedin = 32,
        Wordpress = 64,
        Password = 128,
        Apple = 256,
        NextCloud = 512,
        PersonalAccessToken = 1024,
    };
    Q_ENUM(AuthMethodType)
    Q_DECLARE_FLAGS(AuthMethodTypes, AuthMethodType)

    ~AuthenticationManager() override;

    static AuthenticationManager *self();

    [[nodiscard]] QList<PluginAuthentication *> pluginsList() const;

    [[nodiscard]] PluginAuthentication *findPluginAuthentication(AuthenticationManager::AuthMethodType type);

private:
    explicit AuthenticationManager(QObject *parent = nullptr);
    LIBRUQOLACORE_NO_EXPORT void initializePluginList();
    LIBRUQOLACORE_NO_EXPORT void loadPlugin(AuthenticationManagerInfo *item);
    LIBRUQOLACORE_NO_EXPORT PluginUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QList<AuthenticationManagerInfo> mPluginList;
    QList<PluginUtilData> mPluginDataList;
};
Q_DECLARE_METATYPE(AuthenticationManager::AuthMethodTypes)
Q_DECLARE_METATYPE(AuthenticationManager::AuthMethodType)
