/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

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

    // state == LoginOngoing for all the time since the login request until a response
    //   comes back, then it may result in
    //   - LoggedIn
    //   - LoginOtpRequired
    //   - LoginFailedInvalidUserOrPassword
    //   - GenericError
    // state == LoginOtpAuthOngoing since when the otp code is sent to the server until
    //  a response comes back, then it may become:
    //   - LoggedIn
    //   - LoginFailedInvalidOtp
    //   - GenericError
    // state == LogoutOngoing since the logout request is sent until a response is received,
    //   next states could be
    //   - LoggedOut
    //   - GenericError
    // state == LogoutCleanUpOngoing since the clean up request is sent until a response is
    //   received, resulting in one of these states:
    //   - LoggedOutAndCleanedUp
    //   - GenericError
    // GenericError is used when the class doesn't know what else to do, and is irreversible
    enum LoginStatus : uint8_t {
        Connecting,
        LoginOngoing,
        LoggedIn,
        LoginFailedInvalidUserOrPassword,
        LoginOtpRequired,
        LoginOtpAuthOngoing,
        LoginFailedInvalidOtp,
        LogoutOngoing,
        LoggedOut,
        LogoutCleanUpOngoing,
        LoggedOutAndCleanedUp,
        FailedToLoginPluginProblem,
        LoginFailedUserNotActivated,
        LoginFailedLoginBlockForIp,
        LoginFailedLoginBlockedForUser,
        LoginFailedLoginAppNotAllowedToLogin,
        GenericError,
    };
    Q_ENUM(LoginStatus)

    ~AuthenticationManager() override;

    static AuthenticationManager *self();

    [[nodiscard]] QList<PluginAuthentication *> pluginsList() const;

    [[nodiscard]] PluginAuthentication *findPluginAuthentication(AuthenticationManager::AuthMethodType type);

    [[nodiscard]] static QString loginStatusToText(LoginStatus status);

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
