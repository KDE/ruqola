/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "checknewserverurlwidget.h"
#include "config-ruqola.h"
#include "ddpapi/ddpclient.h"
#include "plugins/pluginauthentication.h"
#include "rocketchatbackend.h"

#include <KBusyIndicatorWidget>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

CheckNewServerUrlWidget::CheckNewServerUrlWidget(QWidget *parent)
    : QWidget{parent}
    , mServerUrl(new QLineEdit(this))
    , mBusyIndicatorWidget(new KBusyIndicatorWidget(this))
    , mFailedError(new KMessageWidget(this))
    , mConnectionPushButton(new QPushButton(i18nc("@action:button", "Connect"), this))
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});

    auto serverUrlLayout = new QHBoxLayout;
    serverUrlLayout->setObjectName(QStringLiteral("serverUrlLayout"));
    serverUrlLayout->setContentsMargins({});
    topLayout->addLayout(serverUrlLayout);

    auto label = new QLabel(i18nc("@label:textbox", "Server URL:"), this);
    label->setObjectName(QStringLiteral("label"));
    serverUrlLayout->addWidget(label);

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setPlaceholderText(QStringLiteral("open.rocket.chat"));
    serverUrlLayout->addWidget(mServerUrl);
    mServerUrl->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mServerUrl);

    mConnectionPushButton->setIcon(QIcon::fromTheme(QStringLiteral("network-connect-symbolic")));
    mConnectionPushButton->setObjectName(QStringLiteral("connectionPushButton"));
    mConnectionPushButton->setEnabled(false);
    serverUrlLayout->addWidget(mConnectionPushButton);

    connect(mConnectionPushButton, &QPushButton::clicked, this, [this]() {
        mConnectionPushButton->setEnabled(false);
        slotTestConnection();
    });
    connect(mServerUrl, &QLineEdit::textChanged, this, [this](const QString &str) {
        mConnectionPushButton->setEnabled(!str.trimmed().isEmpty());
    });

    connect(mServerUrl, &QLineEdit::returnPressed, this, [this]() {
        if (mConnectionPushButton->isEnabled()) {
            mConnectionPushButton->click();
        }
    });

    mBusyIndicatorWidget->setObjectName(QStringLiteral("mBusyIndicatorWidget"));
    serverUrlLayout->addWidget(mBusyIndicatorWidget);

    mFailedError->setObjectName(QStringLiteral("mFailedError"));
    mFailedError->setMessageType(KMessageWidget::Error);
    mFailedError->setCloseButtonVisible(false);

    topLayout->addWidget(mFailedError);
    topLayout->addStretch(1);

    // Hide by default
    mBusyIndicatorWidget->hide();
    mFailedError->hide();
}

CheckNewServerUrlWidget::~CheckNewServerUrlWidget() = default;

void CheckNewServerUrlWidget::slotTestConnection()
{
    const QString serverUrl{mServerUrl->text().trimmed()};
    if (!serverUrl.isEmpty()) {
        mFailedError->hide();
        mBusyIndicatorWidget->show();
        auto ddpClient = new DDPClient(this);
        auto ddpclientAccountParameter = new DDPClient::DDPClientAccountParameter;
        ddpClient->setDDPClientAccountParameter(ddpclientAccountParameter);
        connect(ddpClient, &DDPClient::wsClosedSocketError, this, [this, ddpClient]() {
            mBusyIndicatorWidget->hide();
            mConnectionPushButton->setEnabled(true);
            slotErrorConnection(i18n("Socket was unexpectedly closed."));
            ddpClient->deleteLater();
        });
        connect(ddpClient, &DDPClient::socketError, this, [this, ddpClient](QAbstractSocket::SocketError error, const QString &strError) {
            Q_UNUSED(error);
            Q_UNUSED(strError);
            mConnectionPushButton->setEnabled(true);
            mBusyIndicatorWidget->hide();
            slotErrorConnection(strError);
            ddpClient->deleteLater();
        });

        connect(ddpClient, &DDPClient::methodRequested, this, [this, ddpClient](const QJsonObject &obj, DDPClient::MethodRequestedType type) {
            if (type == DDPClient::MethodRequestedType::PublicSettings) {
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

                Q_EMIT serverUrlFound(std::move(info));
                mBusyIndicatorWidget->hide();
                ddpClient->deleteLater();
            }
        });
        connect(ddpClient, &DDPClient::connectedChanged, this, [ddpClient]() {
            ddpClient->loadPublicSettings();
        });

        ddpClient->setServerUrl(mServerUrl->text());
        ddpClient->start();
    }
}

void CheckNewServerUrlWidget::slotErrorConnection(const QString &message)
{
    mFailedError->setText(i18n("Failed to connect to server: %1", message));
    mFailedError->animatedShow();
}

#include "moc_checknewserverurlwidget.cpp"
