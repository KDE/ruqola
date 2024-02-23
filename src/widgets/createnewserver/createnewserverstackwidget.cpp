/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverstackwidget.h"
#include "createnewservercheckurlwidget.h"
#include "createnewserverwidget.h"
#include "plugins/pluginauthentication.h"
#include "plugins/pluginauthenticationconfigurewidget.h"
#include "plugins/pluginauthenticationinterface.h"
#include "ruqolawidgets_debug.h"

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
    if (auto plugin = AuthenticationManager::self()->findPluginAuthentication(type)) {
        auto interface = plugin->createInterface(this);
        auto configureWidget = interface->configureWidget(this);
        if (mPluginAuthenticationConfigureWidget) {
            removeWidget(mPluginAuthenticationConfigureWidget);
            delete mPluginAuthenticationConfigureWidget;
        }
        mPluginAuthenticationConfigureWidget = configureWidget;
        addWidget(mPluginAuthenticationConfigureWidget);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find authentication for " << type;
    }
}

void CreateNewServerStackWidget::setExistingAccountName(const QStringList &lst)
{
    mCreateNewServerWidget->setExistingAccountName(lst);
}

AccountManager::AccountManagerInfo CreateNewServerStackWidget::accountInfo() const
{
    if (mPluginAuthenticationConfigureWidget) {
        return mPluginAuthenticationConfigureWidget->accountInfo();
    }
    const AccountManager::AccountManagerInfo info = mCreateNewServerWidget->accountInfo();
    return info;
}

void CreateNewServerStackWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mCreateNewServerWidget->setAccountInfo(info);
    setCurrentWidget(mCreateNewServerWidget);
}
#include "moc_createnewserverstackwidget.cpp"
