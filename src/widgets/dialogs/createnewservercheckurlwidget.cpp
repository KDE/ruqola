/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewservercheckurlwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

CreateNewServerCheckUrlWidget::CreateNewServerCheckUrlWidget(QWidget *parent)
    : QWidget{parent}
    , mServerUrl(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Server Url:"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mainLayout->addWidget(mServerUrl);
    new LineEditCatchReturnKey(mServerUrl, this);

    auto connectionPushButton = new QPushButton(i18n("Connection"), this);
    connectionPushButton->setObjectName(QStringLiteral("connectionPushButton"));
    mainLayout->addWidget(connectionPushButton);
    connect(connectionPushButton, &QPushButton::clicked, this, &CreateNewServerCheckUrlWidget::slotTestConnection);

    // TODO add kmessagewidget when we can't connect.
}

CreateNewServerCheckUrlWidget::~CreateNewServerCheckUrlWidget() = default;

void CreateNewServerCheckUrlWidget::slotTestConnection()
{
    // TODO
}

#include "moc_createnewservercheckurlwidget.cpp"
