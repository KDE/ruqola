/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mAuthenticationLoginWidget->setObjectName(u"mAuthenticationLoginWidget"_s);
    mAuthenticationWidget->setObjectName(u"mAuthenticationWidget"_s);
    mainLayout->addWidget(mAuthenticationLoginWidget);
    mainLayout->addWidget(mAuthenticationWidget);
    mainLayout->addStretch(10);
    // TODO add support for two factor ?

    connect(mAuthenticationLoginWidget, &AuthenticationLoginWidget::settingsIsValid, this, &CreateNewServerWidget::settingsIsValid);
    connect(mAuthenticationWidget, &AuthenticationOauthWidget::authentication, this, &CreateNewServerWidget::authentication);
}

CreateNewServerWidget::~CreateNewServerWidget() = default;

AccountManager::AccountManagerInfo CreateNewServerWidget::accountInfo()
{
    return mAuthenticationLoginWidget->accountInfo();
}

void CreateNewServerWidget::setAccountInfo(const AccountManager::AccountManagerInfo &accountInfo)
{
    mAuthenticationLoginWidget->hide();
    mAuthenticationWidget->hide();
    mAuthenticationWidget->clear();
    for (const auto &info : accountInfo.authenticationInfos) {
        if (info.oauthType() == AuthenticationManager::AuthMethodType::Password) {
            mAuthenticationLoginWidget->show();
            mAuthenticationLoginWidget->setAccountInfo(accountInfo);
        } else {
            mAuthenticationWidget->show();
            mAuthenticationWidget->addAuthenticationMethod(info);
        }
    }
}

void CreateNewServerWidget::setExistingAccountNames(const QStringList &lst)
{
    mAuthenticationLoginWidget->setExistingAccountNames(lst);
}

#include "moc_createnewserverwidget.cpp"
