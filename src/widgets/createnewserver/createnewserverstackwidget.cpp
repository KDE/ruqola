/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverstackwidget.h"
#include "checknewserverurlwidget.h"
#include "createnewserverwidget.h"
#include "plugins/pluginauthentication.h"
#include "plugins/pluginauthenticationconfigurewidget.h"
#include "plugins/pluginauthenticationinterface.h"
#include "ruqolawidgets_debug.h"

CreateNewServerStackWidget::CreateNewServerStackWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mCheckNewServerUrlWidget(new CheckNewServerUrlWidget(this))
    , mCreateNewServerWidget(new CreateNewServerWidget(this))
{
    mCheckNewServerUrlWidget->setObjectName(QStringLiteral("mCreateNewServerCheckUrlWidget"));
    addWidget(mCheckNewServerUrlWidget);

    mCreateNewServerWidget->setObjectName(QStringLiteral("mCreateNewServerWidget"));
    addWidget(mCreateNewServerWidget);
    setCurrentWidget(mCheckNewServerUrlWidget);
    connect(mCheckNewServerUrlWidget, &CheckNewServerUrlWidget::serverUrlFound, this, [this](const CheckNewServerUrlWidget::ServerInfo &serverInfo) {
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
    if (type == AuthenticationManager::AuthMethodType::Password) {
        return;
    }
    if (auto plugin = AuthenticationManager::self()->findPluginAuthentication(type)) {
        auto interface = plugin->createInterface(this);
        auto configureWidget = interface->configureWidget(this);
        if (mPluginAuthenticationConfigureWidget) {
            removeWidget(mPluginAuthenticationConfigureWidget);
            delete mPluginAuthenticationConfigureWidget;
        }
        mPluginAuthenticationConfigureWidget = configureWidget;
        mPluginAuthenticationConfigureWidget->setExistingAccountNames(mExistingAccountNames);
        mPluginAuthenticationConfigureWidget->setAccountInfo(mAccountManagerInfo);
        connect(mPluginAuthenticationConfigureWidget, &PluginAuthenticationConfigureWidget::enableOkButton, this, &CreateNewServerStackWidget::updateOkButton);
        addWidget(mPluginAuthenticationConfigureWidget);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find authentication for " << type;
    }
}

void CreateNewServerStackWidget::setExistingAccountNames(const QStringList &lst)
{
    mExistingAccountNames = lst;
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->setExistingAccountNames(lst);
    } else {
        mCreateNewServerWidget->setExistingAccountNames(lst);
    }
}

AccountManager::AccountManagerInfo CreateNewServerStackWidget::accountInfo() const
{
    if (mPluginAuthenticationConfigureWidget) {
        return mPluginAuthenticationConfigureWidget->accountInfo();
    }
    return mCreateNewServerWidget->accountInfo();
}

void CreateNewServerStackWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAccountManagerInfo = info;
    addAuthenticationConfigureWidget(mAccountManagerInfo.authMethodType);
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->setAccountInfo(mAccountManagerInfo);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
    } else {
        mCreateNewServerWidget->setAccountInfo(mAccountManagerInfo);
        setCurrentWidget(mCreateNewServerWidget);
    }
}
#include "moc_createnewserverstackwidget.cpp"
