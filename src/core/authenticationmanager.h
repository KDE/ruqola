/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <KPluginMetaData>
#include <QObject>
#include <QVector>
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
    enum OauthType {
        Unknown = 0,
        Twitter = 1,
        FaceBook = 2,
        GitHub = 4,
        GitLab = 8,
        Google = 16,
        Linkedin = 32,
        Wordpress = 64,
        Password = 128,
    };
    Q_ENUM(OauthType)
    Q_DECLARE_FLAGS(OauthTypes, OauthType)

    explicit AuthenticationManager(QObject *parent = nullptr);
    ~AuthenticationManager() override;

    static AuthenticationManager *self();

    Q_REQUIRED_RESULT QVector<PluginAuthentication *> pluginsList() const;

private:
    Q_DISABLE_COPY(AuthenticationManager)
    void initializePluginList();
    void loadPlugin(AuthenticationManagerInfo *item);
    PluginUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QVector<AuthenticationManagerInfo> mPluginList;
    QVector<PluginUtilData> mPluginDataList;
};
Q_DECLARE_METATYPE(AuthenticationManager::OauthTypes)
Q_DECLARE_METATYPE(AuthenticationManager::OauthType)
