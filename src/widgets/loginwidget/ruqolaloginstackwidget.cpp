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
    // This method is called each time the login status changes, but the widget only needs to be
    // recreated when the authentication method itself changed: it emits tryLogin() (return pressed
    // in the password line edit), so recreating it from that very signal would delete it while it
    // is still emitting. It would also throw away what the user typed in the meantime.
    if (!mPluginAuthenticationConfigureWidget || mAuthMethodType != type) {
        if (auto plugin = AuthenticationManager::self()->findPluginAuthentication(type)) {
            auto interface = plugin->createInterface(this);
            auto configureWidget = interface->configureWidget(this);
            delete interface;
            if (mPluginAuthenticationConfigureWidget) {
                removeWidget(mPluginAuthenticationConfigureWidget);
                mPluginAuthenticationConfigureWidget->deleteLater();
            }
            mPluginAuthenticationConfigureWidget = configureWidget;
            mAuthMethodType = type;
            mPluginAuthenticationConfigureWidget->setAuthenticationLoginType(PluginAuthenticationConfigureWidget::AuthenticationLoginType::Login);
            connect(mPluginAuthenticationConfigureWidget,
                    &PluginAuthenticationConfigureWidget::settingsIsValid,
                    this,
                    &RuqolaLoginStackWidget::settingsIsValid);
            connect(mPluginAuthenticationConfigureWidget, &PluginAuthenticationConfigureWidget::tryLogin, this, &RuqolaLoginStackWidget::tryLogin);
            mPluginAuthenticationConfigureWidget->setExistingAccountNames(mExistingAccountNames);
            addWidget(mPluginAuthenticationConfigureWidget);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find authentication for " << type;
        }
    }
    if (mPluginAuthenticationConfigureWidget) {
        mPluginAuthenticationConfigureWidget->setAccountInfo(mAccountManagerInfo);
        setCurrentWidget(mPluginAuthenticationConfigureWidget);
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
}

#include "moc_ruqolaloginstackwidget.cpp"
