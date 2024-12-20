/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "checknewserverurlwidget.h"
#include "ddpapi/ddpclient.h"
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
        // TODO FIX ME
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

        connect(ddpClient, &DDPClient::methodRequested, this, [](const QJsonObject &obj, DDPClient::MethodRequestedType type) {
            if (type == DDPClient::MethodRequestedType::PublicSettings) {
                qDebug() << " obj " << obj;
            }
        });

        ddpClient->loadPublicSettings();

#if 0 // FIXME
        connect(account, &RocketChatAccount::publicSettingChanged, this, [this, ddpClient, account]() {
            mBusyIndicatorWidget->hide();
            ddpClient->deleteLater();
            account->deleteLater();
            ServerInfo info;
            info.authenticationInfos = account->authenticationMethodInfos();

            const bool allowResetPassword = account->allowPasswordReset() && account->allowPasswordChange();
            info.url = mServerUrl->text().trimmed();
            info.canResetPassword = allowResetPassword;
            info.canRegisterAccount = account->registrationFormEnabled();
            info.passwordSettings = account->ruqolaServerConfig()->passwordSettings();
            info.accountsManuallyApproveNewUsers = account->ruqolaServerConfig()->accountsManuallyApproveNewUsers();
            Q_EMIT serverUrlFound(std::move(info));
        });
#endif

        ddpClient->setServerUrl(mServerUrl->text());
        ddpClient->start();
        ddpClient->loadPublicSettings();
    }
}

void CheckNewServerUrlWidget::slotErrorConnection(const QString &message)
{
    mFailedError->setText(i18n("Failed to connect to server: %1", message));
    mFailedError->animatedShow();
}

#include "moc_checknewserverurlwidget.cpp"
