/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverstackwidget.h"
#include "checknewserverurlwidget.h"
#include "config-ruqola.h"
#if HAVE_ACTIVITY_SUPPORT
#include "configureactivitiesdialog.h"
#endif
#include "createnewserverwidget.h"
#include "plugins/pluginauthentication.h"
#include "plugins/pluginauthenticationconfigurewidget.h"
#include "plugins/pluginauthenticationinterface.h"
#include "ruqolawidgets_debug.h"

#include <QPointer>

CreateNewServerStackWidget::CreateNewServerStackWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mCheckNewServerUrlWidget(new CheckNewServerUrlWidget(this))
    , mCreateNewServerWidget(new CreateNewServerWidget(this))
{
    mCheckNewServerUrlWidget->setObjectName(QStringLiteral("mCheckNewServerUrlWidget"));
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
        info.accountsManuallyApproveNewUsers = serverInfo.accountsManuallyApproveNewUsers;
        info.passwordSettings = serverInfo.passwordSettings;
        setAccountInfo(std::move(info));
    });
    connect(mCreateNewServerWidget, &CreateNewServerWidget::settingsIsValid, this, &CreateNewServerStackWidget::settingsIsValid);
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
        connect(mPluginAuthenticationConfigureWidget,
                &PluginAuthenticationConfigureWidget::settingsIsValid,
                this,
                &CreateNewServerStackWidget::settingsIsValid);
        connect(mPluginAuthenticationConfigureWidget, &PluginAuthenticationConfigureWidget::tryLogin, this, &CreateNewServerStackWidget::tryLogin);
        mPluginAuthenticationConfigureWidget->setExistingAccountNames(mExistingAccountNames);
        mPluginAuthenticationConfigureWidget->setAccountInfo(mAccountManagerInfo);
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
    AccountManager::AccountManagerInfo info;
    if (mPluginAuthenticationConfigureWidget) {
        info = mPluginAuthenticationConfigureWidget->accountInfo();
        info.activitiesSettings = mAccountManagerInfo.activitiesSettings;
        return info;
    }
    info = mCreateNewServerWidget->accountInfo();
    info.activitiesSettings = mAccountManagerInfo.activitiesSettings;
    return info;
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

void CreateNewServerStackWidget::slotConfigureActivities()
{
#if HAVE_ACTIVITY_SUPPORT
    QPointer<ConfigureActivitiesDialog> dlg = new ConfigureActivitiesDialog(this);
    dlg->setActivitiesSettings(mAccountManagerInfo.activitiesSettings);
    if (dlg->exec()) {
        mAccountManagerInfo.activitiesSettings = dlg->activitiesSettings();
    }
    delete dlg;
#endif
}

#include "moc_createnewserverstackwidget.cpp"
