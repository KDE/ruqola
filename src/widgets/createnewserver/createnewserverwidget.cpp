/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverwidget.h"
#include "common/authenticationloginwidget.h"
#include "common/authenticationoauthwidget.h"

#include <KLocalizedString>
#include <QVBoxLayout>

CreateNewServerWidget::CreateNewServerWidget(QWidget *parent)
    : QWidget(parent)
    , mAuthenticationWidget(new AuthenticationOauthWidget(this))
    , mAuthenticationLoginWidget(new AuthenticationLoginWidget(this))
{
    // TODO add icon from server
    // TODO add info from server ?
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAuthenticationLoginWidget->setObjectName(QStringLiteral("mAuthenticationLoginWidget"));
    mAuthenticationWidget->setObjectName(QStringLiteral("mAuthenticationWidget"));
    mainLayout->addWidget(mAuthenticationLoginWidget);
    // TODO update list of authentication.
    mainLayout->addWidget(mAuthenticationWidget);
    // TODO add support for two factor ?

    connect(mAuthenticationLoginWidget, &AuthenticationLoginWidget::updateOkButton, this, &CreateNewServerWidget::updateOkButton);
}

CreateNewServerWidget::~CreateNewServerWidget() = default;

AccountManager::AccountManagerInfo CreateNewServerWidget::accountInfo()
{
    return mAuthenticationLoginWidget->accountInfo();
}

void CreateNewServerWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAuthenticationLoginWidget->setAccountInfo(info);
}

void CreateNewServerWidget::setExistingAccountName(const QStringList &lst)
{
    mAuthenticationLoginWidget->setExistingAccountName(lst);
}

void CreateNewServerWidget::setAuthenticationInfos(const QVector<AuthenticationInfo> &authenticationInfos)
{
    mAuthenticationLoginWidget->hide();
    mAuthenticationWidget->hide();
    for (const auto &info : authenticationInfos) {
        if (info.oauthType() == AuthenticationManager::AuthMethodType::Password) {
            mAuthenticationLoginWidget->show();
        } else {
            mAuthenticationWidget->show();
            mAuthenticationWidget->addAuthenticationMethod(info);
        }
    }
}

#include "moc_createnewserverwidget.cpp"
