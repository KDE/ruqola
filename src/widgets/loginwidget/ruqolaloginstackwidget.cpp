/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginstackwidget.h"
#include "plugins/pluginauthentication.h"
#include "plugins/pluginauthenticationconfigurewidget.h"
#include "plugins/pluginauthenticationinterface.h"
#include "ruqolawidgets_debug.h"

RuqolaLoginStackWidget::RuqolaLoginStackWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType::Password);
}

RuqolaLoginStackWidget::~RuqolaLoginStackWidget() = default;

void RuqolaLoginStackWidget::changeAuthenticationWidgetStatus(bool enabled)
{
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->changeAuthenticationWidgetStatus(enabled);
    }
}

void RuqolaLoginStackWidget::addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type)
{
    if (auto plugin = AuthenticationManager::self()->findPluginAuthentication(type)) {
        auto interface = plugin->createInterface(this);
        auto configureWidget = interface->configureWidget(this);
        if (mPluginAuthenticationConfigureWidget) {
            removeWidget(mPluginAuthenticationConfigureWidget);
            delete mPluginAuthenticationConfigureWidget;
        }
        mPluginAuthenticationConfigureWidget = configureWidget;
        mPluginAuthenticationConfigureWidget->setAuthenticationLoginType(PluginAuthenticationConfigureWidget::AuthenticationLoginType::Login);
        connect(mPluginAuthenticationConfigureWidget, &PluginAuthenticationConfigureWidget::settingsIsValid, this, &RuqolaLoginStackWidget::settingsIsValid);
        connect(mPluginAuthenticationConfigureWidget, &PluginAuthenticationConfigureWidget::tryLogin, this, &RuqolaLoginStackWidget::tryLogin);

        mPluginAuthenticationConfigureWidget->setExistingAccountNames(mExistingAccountNames);
        mPluginAuthenticationConfigureWidget->setAccountInfo(mAccountManagerInfo);
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
    return {};
}

void RuqolaLoginStackWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAccountManagerInfo = info;
    addAuthenticationConfigureWidget(mAccountManagerInfo.authMethodType);
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->setAccountInfo(mAccountManagerInfo);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
    }
}

#include "moc_ruqolaloginstackwidget.cpp"
