/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewservercheckurlwidget.h"
#include "ddpapi/ddpclient.h"
#include "misc/lineeditcatchreturnkey.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include <KBusyIndicatorWidget>
#include <KLocalizedString>
#include <KMessageWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

CreateNewServerCheckUrlWidget::CreateNewServerCheckUrlWidget(QWidget *parent)
    : QWidget{parent}
    , mServerUrl(new QLineEdit(this))
    , mBusyIndicatorWidget(new KBusyIndicatorWidget(this))
    , mFailedError(new KMessageWidget(this))
    , mConnectionPushButton(new QPushButton(i18n("Connection"), this))
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});

    auto serverUrlLayout = new QHBoxLayout;
    serverUrlLayout->setObjectName(QStringLiteral("serverUrlLayout"));
    serverUrlLayout->setContentsMargins({});
    topLayout->addLayout(serverUrlLayout);

    auto label = new QLabel(i18n("Server Url:"), this);
    label->setObjectName(QStringLiteral("label"));
    serverUrlLayout->addWidget(label);

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    serverUrlLayout->addWidget(mServerUrl);
    mServerUrl->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mServerUrl, this);

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

CreateNewServerCheckUrlWidget::~CreateNewServerCheckUrlWidget() = default;

void CreateNewServerCheckUrlWidget::slotTestConnection()
{
    const QString serverUrl{mServerUrl->text().trimmed()};
    if (!serverUrl.isEmpty()) {
        mFailedError->hide();
        mBusyIndicatorWidget->show();
        auto account = new RocketChatAccount();
        account->setServerUrl(mServerUrl->text());
        auto ddpClient = new DDPClient(account, this);
        connect(ddpClient, &DDPClient::wsClosedSocketError, this, [this, ddpClient, account]() {
            mBusyIndicatorWidget->hide();
            mConnectionPushButton->setEnabled(true);
            slotErrorConnection();
            ddpClient->deleteLater();
            account->deleteLater();
        });
        connect(ddpClient, &DDPClient::socketError, this, [this, ddpClient, account](QAbstractSocket::SocketError error, const QString &strError) {
            Q_UNUSED(error);
            Q_UNUSED(strError);
            mConnectionPushButton->setEnabled(true);
            mBusyIndicatorWidget->hide();
            slotErrorConnection();
            ddpClient->deleteLater();
            account->deleteLater();
        });
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
            Q_EMIT serverUrlFound(info);
        });

        ddpClient->setServerUrl(mServerUrl->text());
        ddpClient->start();
        account->rocketChatBackend()->loadPublicSettings();
    }
}

void CreateNewServerCheckUrlWidget::slotErrorConnection()
{
    mFailedError->setText(i18n("Impossible to access to server."));
    mFailedError->animatedShow();
}

#include "moc_createnewservercheckurlwidget.cpp"
