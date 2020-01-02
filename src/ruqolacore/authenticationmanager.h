/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

#include <KPluginMetaData>
#include <QObject>
#include "libruqolacore_export.h"
#include <QVector>
class PluginAuthentication;
class PluginUtilData
{
public:
    PluginUtilData()
    {
    }

    QStringList mExtraInfo;
    QString mIdentifier;
    QString mName;
};

class AuthenticationManagerInfo
{
public:
    AuthenticationManagerInfo()
    {
    }

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
        Password = 128
    };
    Q_ENUMS(OauthType)
    Q_DECLARE_FLAGS(OauthTypes, OauthType)

    explicit AuthenticationManager(QObject *parent = nullptr);
    ~AuthenticationManager();

    static AuthenticationManager *self();

    Q_REQUIRED_RESULT QVector<PluginAuthentication *> pluginsList() const;

private:
    Q_DISABLE_COPY(AuthenticationManager)
    bool initializePluginList();
    void loadPlugin(AuthenticationManagerInfo *item);
    PluginUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QVector<AuthenticationManagerInfo> mPluginList;
    QVector<PluginUtilData> mPluginDataList;
};
Q_DECLARE_METATYPE(AuthenticationManager::OauthTypes)
Q_DECLARE_METATYPE(AuthenticationManager::OauthType)

#endif // AUTHENTICATIONMANAGER_H
