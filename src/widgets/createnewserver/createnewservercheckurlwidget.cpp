/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewservercheckurlwidget.h"
#include "colors.h"
#include "misc/lineeditcatchreturnkey.h"
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

    auto serverUrlLayout = new QHBoxLayout(this);
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
    serverUrlLayout->addWidget(connectionPushButton);
    connect(connectionPushButton, &QPushButton::clicked, this, &CreateNewServerCheckUrlWidget::slotTestConnection);

    mBusyIndicatorWidget->setObjectName(QStringLiteral("mBusyIndicatorWidget"));
    serverUrlLayout->addWidget(mBusyIndicatorWidget);
    // Hide by default
    mBusyIndicatorWidget->hide();

    mFailedError->setObjectName(QStringLiteral("mFailedError"));
    mFailedError->setMessageType(KMessageWidget::Error);

    topLayout->addWidget(mFailedError);
    // Hide by default
    mFailedError->hide();
}

CreateNewServerCheckUrlWidget::~CreateNewServerCheckUrlWidget() = default;

void CreateNewServerCheckUrlWidget::slotTestConnection()
{
    // TODO
}

void CreateNewServerCheckUrlWidget::slotErrorConnection()
{
    mFailedError->animatedShow();
    // TODO
}

#include "moc_createnewservercheckurlwidget.cpp"
