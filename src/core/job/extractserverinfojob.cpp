/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "extractserverinfojob.h"
#include "ddpapi/ddpclient.h"
#include "plugins/pluginauthentication.h"
#include <KLocalizedString>

ExtractServerInfoJob::ExtractServerInfoJob(QObject *parent)
    : QObject{parent}
{
}

bool ExtractServerInfoJob::canStart() const
{
    return !mServerUrl.trimmed().isEmpty();
}

ExtractServerInfoJob::~ExtractServerInfoJob() = default;

void ExtractServerInfoJob::start()
{
    if (!canStart()) {
        Q_EMIT errorConnection(i18n("Missing server url."));
        deleteLater();
        return;
    }
    auto ddpClient = new DDPClient(this);
    auto ddpclientAccountParameter = new DDPClient::DDPClientAccountParameter;
    ddpClient->setDDPClientAccountParameter(ddpclientAccountParameter);
    connect(ddpClient, &DDPClient::wsClosedSocketError, this, [this, ddpClient]() {
        Q_EMIT errorConnection(i18n("Socket was unexpectedly closed."));
        ddpClient->deleteLater();
    });
    connect(ddpClient, &DDPClient::socketError, this, [this, ddpClient]([[maybe_unused]] QAbstractSocket::SocketError error, const QString &strError) {
        Q_EMIT errorConnection(strError);
        ddpClient->deleteLater();
        deleteLater();
    });

    connect(ddpClient, &DDPClient::methodRequested, this, [this, ddpClient](const QJsonObject &obj, DDPClient::MethodRequestedType type) {
        if (type == DDPClient::MethodRequestedType::PublicSettings) {
            RuqolaServerConfig config;
            config.parsePublicSettings(obj, false);
            // qDebug() << " obj " << obj;

            ServerInfo info;
            info.url = mServerUrl;
            info.canRegisterAccount = config.serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::RegistrationFormEnabled;
            info.passwordSettings = config.passwordSettings();
            info.accountsManuallyApproveNewUsers = config.accountsManuallyApproveNewUsers();

            const bool allowResetPassword = (config.serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowPasswordChange)
                && (config.serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowPasswordReset);
            info.canResetPassword = allowResetPassword;

            const QList<PluginAuthentication *> lstPlugins = AuthenticationManager::self()->pluginsList();
            QList<AuthenticationInfo> authenticationMethodInfos;
            for (PluginAuthentication *abstractPlugin : lstPlugins) {
                AuthenticationInfo authenticationInfo;
                authenticationInfo.setIconName(abstractPlugin->iconName());
                authenticationInfo.setName(abstractPlugin->name());
                authenticationInfo.setOauthType(abstractPlugin->authenticationType());
                if (authenticationInfo.isValid()) {
                    authenticationMethodInfos.append(std::move(authenticationInfo));
                }
                config.addRuqolaAuthenticationSupport(abstractPlugin->authenticationType());
            }
            QList<AuthenticationInfo> fillModel;
            // qDebug() << " before " << authenticationMethodInfos;
            for (int i = 0, total = authenticationMethodInfos.count(); i < total; ++i) {
                if (config.canShowAuthMethod(authenticationMethodInfos.at(i).oauthType())
                    || (authenticationMethodInfos.at(i).oauthType() == AuthenticationManager::AuthMethodType::PersonalAccessToken)) {
                    fillModel.append(authenticationMethodInfos.at(i));
                }
            }
            info.authenticationInfos = fillModel;

            Q_EMIT serverInfoFound(info);
            ddpClient->deleteLater();
            deleteLater();
        }
    });
    connect(ddpClient, &DDPClient::connectedChanged, this, [ddpClient]() {
        ddpClient->loadPublicSettings();
    });

    ddpClient->setServerUrl(mServerUrl);
    ddpClient->start();
}

QString ExtractServerInfoJob::serverUrl() const
{
    return mServerUrl;
}

void ExtractServerInfoJob::setServerUrl(const QString &newServerUrl)
{
    mServerUrl = newServerUrl;
}

#include "moc_extractserverinfojob.cpp"
