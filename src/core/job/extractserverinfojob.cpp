/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

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
        // TODO emit signal
        return;
    }
    auto ddpClient = new DDPClient(this);
    auto ddpclientAccountParameter = new DDPClient::DDPClientAccountParameter;
    ddpClient->setDDPClientAccountParameter(ddpclientAccountParameter);
    connect(ddpClient, &DDPClient::wsClosedSocketError, this, [this, ddpClient]() {
        Q_EMIT errorConnection(i18n("Socket was unexpectedly closed."));
        ddpClient->deleteLater();
    });
    connect(ddpClient, &DDPClient::socketError, this, [this, ddpClient](QAbstractSocket::SocketError error, const QString &strError) {
        Q_UNUSED(error);
        Q_EMIT errorConnection(strError);
        ddpClient->deleteLater();
    });

    connect(ddpClient, &DDPClient::methodRequested, this, [this, ddpClient](const QJsonObject &obj, DDPClient::MethodRequestedType type) {
        if (type == DDPClient::MethodRequestedType::PublicSettings) {
            RuqolaServerConfig config;
            config.parsePublicSettings(obj, false);
            // TODO parse info
            // qDebug() << " obj " << obj;

            ServerInfo info;
            info.url = mServerUrl.trimmed();
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

            Q_EMIT serverInfoFound(std::move(info));
            ddpClient->deleteLater();
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
