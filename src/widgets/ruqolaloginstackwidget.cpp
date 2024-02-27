/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginstackwidget.h"
#include "configurenewserver/createnewserverwidget.h"
#include "plugins/pluginauthentication.h"
#include "plugins/pluginauthenticationconfigurewidget.h"
#include "plugins/pluginauthenticationinterface.h"
#include "ruqolawidgets_debug.h"

RuqolaLoginStackWidget::RuqolaLoginStackWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mCreateNewServerWidget(new CreateNewServerWidget(this))
{
    mCreateNewServerWidget->setObjectName(QStringLiteral("mCreateNewServerWidget"));
    addWidget(mCreateNewServerWidget);
}

RuqolaLoginStackWidget::~RuqolaLoginStackWidget() = default;

void RuqolaLoginStackWidget::addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type)
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
        // TODO connect(mPluginAuthenticationConfigureWidget, &PluginAuthenticationConfigureWidget::enableOkButton, this,
        // &CreateNewServerStackWidget::updateOkButton);
        addWidget(mPluginAuthenticationConfigureWidget);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find authentication for " << type;
    }
}

void RuqolaLoginStackWidget::setExistingAccountNames(const QStringList &lst)
{
    mExistingAccountNames = lst;
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->setExistingAccountNames(lst);
    } else {
        mCreateNewServerWidget->setExistingAccountNames(lst);
    }
}

AccountManager::AccountManagerInfo RuqolaLoginStackWidget::accountInfo() const
{
    if (mPluginAuthenticationConfigureWidget) {
        return mPluginAuthenticationConfigureWidget->accountInfo();
    }
    return mCreateNewServerWidget->accountInfo();
}

void RuqolaLoginStackWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
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
#include "moc_ruqolaloginstackwidget.cpp"
