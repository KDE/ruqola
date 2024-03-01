/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginstackwidget.h"
#include "common/authenticationloginwidget.h"
#include "plugins/pluginauthentication.h"
#include "plugins/pluginauthenticationconfigurewidget.h"
#include "plugins/pluginauthenticationinterface.h"
#include "ruqolawidgets_debug.h"

RuqolaLoginStackWidget::RuqolaLoginStackWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mAuthenticationLoginWidget(new AuthenticationLoginWidget(this))
{
    mAuthenticationLoginWidget->setObjectName(QStringLiteral("mAuthenticationLoginWidget"));
    addWidget(mAuthenticationLoginWidget);
    mAuthenticationLoginWidget->setAuthenticationLoginType(AuthenticationLoginWidget::AuthenticationLoginType::Login);
}

RuqolaLoginStackWidget::~RuqolaLoginStackWidget() = default;

void RuqolaLoginStackWidget::changeAuthenticationWidgetStatus(bool enabled)
{
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->changeAuthenticationWidgetStatus(enabled);
    } else {
        mAuthenticationLoginWidget->changeAuthenticationWidgetStatus(enabled);
    }
}

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
        connect(mPluginAuthenticationConfigureWidget, &PluginAuthenticationConfigureWidget::settingsIsValid, this, &RuqolaLoginStackWidget::settingsIsValid);
        addWidget(mPluginAuthenticationConfigureWidget);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
        delete interface;
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find authentication for " << type;
    }
}

AccountManager::AccountManagerInfo RuqolaLoginStackWidget::accountInfo() const
{
    if (mPluginAuthenticationConfigureWidget) {
        return mPluginAuthenticationConfigureWidget->accountInfo();
    }
    return mAuthenticationLoginWidget->accountInfo();
}

void RuqolaLoginStackWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAccountManagerInfo = info;
    addAuthenticationConfigureWidget(mAccountManagerInfo.authMethodType);
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->setAccountInfo(mAccountManagerInfo);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
    } else {
        mAuthenticationLoginWidget->setAccountInfo(mAccountManagerInfo);
        setCurrentWidget(mAuthenticationLoginWidget);
    }
}

void RuqolaLoginStackWidget::setAuthenticationLoginType(AuthenticationLoginWidget::AuthenticationLoginType type)
{
    mAuthenticationLoginWidget->setAuthenticationLoginType(type);
}

#include "moc_ruqolaloginstackwidget.cpp"
