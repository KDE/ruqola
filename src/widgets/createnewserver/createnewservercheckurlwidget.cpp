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
    new LineEditCatchReturnKey(mServerUrl, this);

    auto connectionPushButton = new QPushButton(i18n("Connection"), this);
    connectionPushButton->setObjectName(QStringLiteral("connectionPushButton"));
    connectionPushButton->setEnabled(false);
    serverUrlLayout->addWidget(connectionPushButton);

    connect(connectionPushButton, &QPushButton::clicked, this, &CreateNewServerCheckUrlWidget::slotTestConnection);
    connect(mServerUrl, &QLineEdit::textChanged, this, [connectionPushButton](const QString &str) {
        connectionPushButton->setEnabled(!str.trimmed().isEmpty());
    });

    mBusyIndicatorWidget->setObjectName(QStringLiteral("mBusyIndicatorWidget"));
    serverUrlLayout->addWidget(mBusyIndicatorWidget);
    // Hide by default
    mBusyIndicatorWidget->hide();

    mFailedError->setObjectName(QStringLiteral("mFailedError"));
    mFailedError->setMessageType(KMessageWidget::Error);

    topLayout->addWidget(mFailedError);
    // Hide by default
    mFailedError->hide();
    topLayout->addStretch(1);
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
        connect(ddpClient, &DDPClient::socketError, this, [this, ddpClient, account](QAbstractSocket::SocketError error, const QString &strError) {
            mBusyIndicatorWidget->hide();
            slotErrorConnection();
            ddpClient->deleteLater();
            account->deleteLater();
        });
        connect(account, &RocketChatAccount::publicSettingChanged, this, [this, ddpClient, account]() {
            slotSuccessConnection();
            mBusyIndicatorWidget->hide();
            slotErrorConnection();
            ddpClient->deleteLater();
            account->deleteLater();
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

void CreateNewServerCheckUrlWidget::slotSuccessConnection()
{
    mBusyIndicatorWidget->hide();
    Q_EMIT serverUrlFound(mServerUrl->text().trimmed());
}

#include "moc_createnewservercheckurlwidget.cpp"
