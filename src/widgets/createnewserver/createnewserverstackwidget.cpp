/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
            [this](const QString &serverUrl, const QVector<AuthenticationInfo> &authenticationInfos, bool canResetPassword) {
                AccountManager::AccountManagerInfo info;
                info.serverUrl = serverUrl;
                info.authenticationInfos = authenticationInfos;
                info.canResetPassword = canResetPassword;
                setAccountInfo(std::move(info));
            });
    connect(mCreateNewServerWidget, &CreateNewServerWidget::updateOkButton, this, &CreateNewServerStackWidget::updateOkButton);
    connect(mCreateNewServerWidget, &CreateNewServerWidget::authentication, this, &CreateNewServerStackWidget::authentication);
}

CreateNewServerStackWidget::~CreateNewServerStackWidget() = default;

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
