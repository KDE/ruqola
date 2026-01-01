/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restauthenticationgui.h"
#include "authenticationmanager/restauthenticationmanager.h"
#include "common/authenticationloginwidget.h"
#include "connection.h"
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

    auto login = new QPushButton(u"Login"_s, this);
    mainLayout->addWidget(login);
    connect(login, &QPushButton::clicked, this, [this, dummyAccount, authManager]() {
        const AccountManager::AccountManagerInfo info = mAuthenticationLoginWidget->accountInfo();
        dummyAccount->restApi()->setServerUrl(info.serverUrl);
        if (!authManager->loginPassword(info.userName, info.password)) {
            qDebug() << " Impossible to log";
        }
    });
    auto logout = new QPushButton(u"Logout"_s, this);
    mainLayout->addWidget(logout);
    connect(logout, &QPushButton::clicked, this, [this, dummyAccount, authManager]() {
        const AccountManager::AccountManagerInfo info = mAuthenticationLoginWidget->accountInfo();
        dummyAccount->restApi()->setServerUrl(info.serverUrl);
        authManager->logout();
    });
    auto label = new QLabel(this);
    mainLayout->addWidget(label);
    connect(authManager, &RESTAuthenticationManager::loginStatusChanged, this, [label, authManager]() {
        label->setText(u"Login Status: %1"_s.arg(authManager->loginStatus()));
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
