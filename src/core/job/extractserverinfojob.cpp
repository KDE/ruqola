/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "extractserverinfojob.h"
#include "ruqolaserverconfig.h"

ExtractServerInfoJob::ExtractServerInfoJob(QObject *parent)
    : QObject{parent}
{
}

ExtractServerInfoJob::~ExtractServerInfoJob() = default;

void ExtractServerInfoJob::start()
{
#if 0
    RuqolaServerConfig config;
    config.parsePublicSettings(obj, false);
    // TODO parse info
    // qDebug() << " obj " << obj;

    ServerInfo info;
    info.url = mServerUrl->text().trimmed();
    info.canRegisterAccount = config.serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::RegistrationFormEnabled;
    info.passwordSettings = config.passwordSettings();
    info.accountsManuallyApproveNewUsers = config.accountsManuallyApproveNewUsers();

    const bool allowResetPassword = (config.serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowPasswordChange)
        && (config.serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowPasswordReset);
    info.canResetPassword = allowResetPassword;

    const QList<PluginAuthentication *> lstPlugins = AuthenticationManager::self()->pluginsList();
    QList<AuthenticationInfo> authenticationMethodInfos;
    for (PluginAuthentication *abstractPlugin : lstPlugins) {
        AuthenticationInfo info;
        info.setIconName(abstractPlugin->iconName());
        info.setName(abstractPlugin->name());
        info.setOauthType(abstractPlugin->authenticationType());
        if (info.isValid()) {
            authenticationMethodInfos.append(std::move(info));
        }
    }
#if 0
    QList<AuthenticationInfo> fillModel;
    qDebug() << " before " << authenticationMethodInfos;
    for (int i = 0, total = authenticationMethodInfos.count(); i < total; ++i) {
        if (config.canShowAuthMethod(authenticationMethodInfos.at(i).oauthType())
        // Reactivate it we will want to show PersonalAccessToken
#if USE_PERSONAL_ACCESS_TOKEN
            || (authenticationMethodInfos.at(i).oauthType() == AuthenticationManager::AuthMethodType::PersonalAccessToken)
#endif
        ) {
            fillModel.append(authenticationMethodInfos.at(i));
            qDebug() << " xcdddddddddddddd " << fillModel;
        }
    }
#else
    // TODO fixme
    QList<AuthenticationInfo> fillModel = authenticationMethodInfos;
#endif
    info.authenticationInfos = fillModel;
#endif
}
