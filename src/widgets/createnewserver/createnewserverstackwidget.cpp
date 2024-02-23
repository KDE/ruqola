/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverstackwidget.h"
#include "createnewservercheckurlwidget.h"
#include "createnewserverwidget.h"

CreateNewServerStackWidget::CreateNewServerStackWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mCreateNewServerCheckUrlWidget(new CreateNewServerCheckUrlWidget(this))
    , mCreateNewServerWidget(new CreateNewServerWidget(this))
{
    mCreateNewServerCheckUrlWidget->setObjectName(QStringLiteral("mCreateNewServerCheckUrlWidget"));
    addWidget(mCreateNewServerCheckUrlWidget);

    mCreateNewServerWidget->setObjectName(QStringLiteral("mCreateNewServerWidget"));
    addWidget(mCreateNewServerWidget);
    setCurrentWidget(mCreateNewServerCheckUrlWidget);
    connect(mCreateNewServerCheckUrlWidget,
            &CreateNewServerCheckUrlWidget::serverUrlFound,
            this,
            [this](const CreateNewServerCheckUrlWidget::ServerInfo &serverInfo) {
                AccountManager::AccountManagerInfo info;
                info.serverUrl = serverInfo.url;
                info.authenticationInfos = serverInfo.authenticationInfos;
                info.canResetPassword = serverInfo.canResetPassword;
                info.canRegisterAccount = serverInfo.canRegisterAccount;
                setAccountInfo(std::move(info));
            });
    connect(mCreateNewServerWidget, &CreateNewServerWidget::updateOkButton, this, &CreateNewServerStackWidget::updateOkButton);
    connect(mCreateNewServerWidget, &CreateNewServerWidget::authentication, this, &CreateNewServerStackWidget::addAuthenticationConfigureWidget);
}

CreateNewServerStackWidget::~CreateNewServerStackWidget() = default;

void CreateNewServerStackWidget::addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type)
{
    // TODO
}

void CreateNewServerStackWidget::setExistingAccountName(const QStringList &lst)
{
    mCreateNewServerWidget->setExistingAccountName(lst);
}

AccountManager::AccountManagerInfo CreateNewServerStackWidget::accountInfo() const
{
    const AccountManager::AccountManagerInfo info = mCreateNewServerWidget->accountInfo();
    return info;
}

void CreateNewServerStackWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mCreateNewServerWidget->setAccountInfo(info);
    setCurrentWidget(mCreateNewServerWidget);
}
#include "moc_createnewserverstackwidget.cpp"
