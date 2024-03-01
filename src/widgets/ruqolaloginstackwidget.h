/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "authenticationmanager.h"
#include "common/authenticationloginwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>

class PluginAuthenticationConfigureWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginStackWidget(QWidget *parent = nullptr);
    ~RuqolaLoginStackWidget() override;

    void addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type);

    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;
    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

    void setAuthenticationLoginType(AuthenticationLoginWidget::AuthenticationLoginType type);

    void changeAuthenticationWidgetStatus(bool enabled);

Q_SIGNALS:
    void settingsIsValid(bool state);
    void tryLogin();

private:
    QStringList mExistingAccountNames;
    AccountManager::AccountManagerInfo mAccountManagerInfo;
    AuthenticationLoginWidget *const mAuthenticationLoginWidget;
    PluginAuthenticationConfigureWidget *mPluginAuthenticationConfigureWidget = nullptr;
};
