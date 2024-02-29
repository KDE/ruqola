/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restauthenticationgui.h"
#include "common/authenticationloginwidget.h"
#include "connection.h"
#include "restauthenticationmanager.h"
#include "rocketchataccount.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

RestAuthenticationGui::RestAuthenticationGui(QWidget *parent)
    : QWidget(parent)
    , mAuthenticationLoginWidget(new AuthenticationLoginWidget(this))
{
    auto dummyAccount = new RocketChatAccount;
    auto authManager = new RESTAuthenticationManager(dummyAccount->restApi());
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mAuthenticationLoginWidget);
    mAuthenticationLoginWidget->setAuthenticationLoginType(AuthenticationLoginWidget::AuthenticationLoginType::Create);

    auto login = new QPushButton(QStringLiteral("Login"), this);
    mainLayout->addWidget(login);
    connect(login, &QPushButton::clicked, this, [this, dummyAccount, authManager]() {
        const AccountManager::AccountManagerInfo info = mAuthenticationLoginWidget->accountInfo();
        dummyAccount->restApi()->setServerUrl(info.serverUrl);
        authManager->login(info.userName, info.password);
    });
}

RestAuthenticationGui::~RestAuthenticationGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    RestAuthenticationGui w;
    w.show();
    return app.exec();
}

#include "moc_restauthenticationgui.cpp"
