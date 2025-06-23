/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanager.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <KPluginFactory>
#include <KPluginMetaData>
#include <QFileInfo>

#include "plugins/pluginauthentication.h"

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
    const QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(u"ruqolaplugins/authentication"_s);

    QListIterator<KPluginMetaData> i(plugins);
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
    QList<AuthenticationManagerInfo>::iterator end(mPluginList.end());
    for (QList<AuthenticationManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
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

PluginAuthentication *AuthenticationManager::findPluginAuthentication(AuthenticationManager::AuthMethodType type)
{
    QList<AuthenticationManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<AuthenticationManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            if (plugin->authenticationType() == type) {
                return plugin;
            }
        }
    }
    return nullptr;
}

QString AuthenticationManager::loginStatusToText(LoginStatus status)
{
    switch (status) {
    case AuthenticationManager::Connecting:
        return i18n("Connecting");
    case AuthenticationManager::LoginOtpAuthOngoing:
        return i18n("Login OTP code required");
    case AuthenticationManager::LoginFailedInvalidUserOrPassword:
        return i18n("Login failed: invalid username or password");
    case AuthenticationManager::LoginOngoing:
        return i18n("Logging in");
    case AuthenticationManager::LoggedIn:
        return i18n("Logged in");
    case AuthenticationManager::LoggedOut:
        return i18n("Logged out");
    case AuthenticationManager::FailedToLoginPluginProblem:
        return i18n("Failed to login due to plugin problem");
    case AuthenticationManager::GenericError:
        return i18n("Login failed: generic error");
    case AuthenticationManager::LoginOtpRequired:
        return i18n("A one-time password is required to complete the login procedure.");
    case AuthenticationManager::LoginFailedInvalidOtp:
        return i18n("Login failed: Invalid OTP code.");
    case AuthenticationManager::LoginFailedUserNotActivated:
        return i18n("Login failed: User is not activated.");
    case AuthenticationManager::LoginFailedLoginBlockForIp:
        return i18n("Login has been temporarily blocked For IP.");
    case AuthenticationManager::LoginFailedLoginBlockedForUser:
        return i18n("Login has been temporarily blocked For User.");
    case AuthenticationManager::LoginFailedLoginAppNotAllowedToLogin:
        return i18n("App user is not allowed to login.");
    case AuthenticationManager::LogoutOngoing:
        return i18n("Logout ongoing");
    case AuthenticationManager::LogoutCleanUpOngoing:
        return i18n("Logout cleanup ongoing");
    case AuthenticationManager::LoggedOutAndCleanedUp:
        return i18n("Successfully logged out");
    }
    return i18n("Unknown state");
}

QList<PluginAuthentication *> AuthenticationManager::pluginsList() const
{
    QList<PluginAuthentication *> lst;
    QList<AuthenticationManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<AuthenticationManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
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

#include "moc_authenticationmanager.cpp"
