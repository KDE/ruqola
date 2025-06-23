/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "checknewserverurlwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "config-ruqola.h"

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
    topLayout->setObjectName(u"topLayout"_s);
    topLayout->setContentsMargins({});

    auto serverUrlLayout = new QHBoxLayout;
    serverUrlLayout->setObjectName(u"serverUrlLayout"_s);
    serverUrlLayout->setContentsMargins({});
    topLayout->addLayout(serverUrlLayout);

    auto label = new QLabel(i18nc("@label:textbox", "Server URL:"), this);
    label->setObjectName(u"label"_s);
    serverUrlLayout->addWidget(label);

    mServerUrl->setObjectName(u"mServerUrl"_s);
    mServerUrl->setPlaceholderText(u"open.rocket.chat"_s);
    serverUrlLayout->addWidget(mServerUrl);
    mServerUrl->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mServerUrl);

    mConnectionPushButton->setIcon(QIcon::fromTheme(u"network-connect-symbolic"_s));
    mConnectionPushButton->setObjectName(u"connectionPushButton"_s);
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

    mBusyIndicatorWidget->setObjectName(u"mBusyIndicatorWidget"_s);
    serverUrlLayout->addWidget(mBusyIndicatorWidget);

    mFailedError->setObjectName(u"mFailedError"_s);
    mFailedError->setMessageType(KMessageWidget::Error);
    mFailedError->setCloseButtonVisible(false);

    topLayout->addWidget(mFailedError);
    topLayout->addStretch(1);

    // Hide by default
    mBusyIndicatorWidget->hide();
    mFailedError->hide();
}

void CheckNewServerUrlWidget::setServerUrl(const QString &serverUrl)
{
    mServerUrl->setText(serverUrl);
    mConnectionPushButton->click();
}

CheckNewServerUrlWidget::~CheckNewServerUrlWidget() = default;

void CheckNewServerUrlWidget::slotTestConnection()
{
    const QString serverUrl{mServerUrl->text().trimmed()};
    if (!serverUrl.isEmpty()) {
        mFailedError->hide();
        mBusyIndicatorWidget->show();
        auto job = new ExtractServerInfoJob(this);
        job->setServerUrl(mServerUrl->text());
        connect(job, &ExtractServerInfoJob::errorConnection, this, [this](const QString &errorStr) {
            mConnectionPushButton->setEnabled(true);
            mBusyIndicatorWidget->hide();
            slotErrorConnection(errorStr);
        });
        connect(job, &ExtractServerInfoJob::serverInfoFound, this, [this](const ExtractServerInfoJob::ServerInfo &info) {
            Q_EMIT serverUrlFound(std::move(info));
            mBusyIndicatorWidget->hide();
        });
        job->start();
    }
}

void CheckNewServerUrlWidget::slotErrorConnection(const QString &message)
{
    mFailedError->setText(i18n("Failed to connect to server: %1", message));
    mFailedError->animatedShow();
}

#include "moc_checknewserverurlwidget.cpp"
