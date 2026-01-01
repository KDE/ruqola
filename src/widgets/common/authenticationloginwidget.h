/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_export.h"
#include "plugins/pluginauthenticationconfigurewidget.h"
class QLineEdit;
class QPushButton;
class PasswordLineEditWidget;
class LIBRUQOLAWIDGETS_EXPORT AuthenticationLoginWidget : public PluginAuthenticationConfigureWidget
{
    Q_OBJECT
public:
    explicit AuthenticationLoginWidget(QWidget *parent = nullptr);
    ~AuthenticationLoginWidget() override;

    void setAccountInfo(const AccountManager::AccountManagerInfo &info) override;
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() override;

    void changeAuthenticationWidgetStatus(bool enabled) override;

    void setAuthenticationLoginType(AuthenticationLoginType type) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetPasswordRequested(const QString &email);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegisterUserDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoginSettingsChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegisterAccount();
    AccountManager::AccountManagerInfo mAccountInfo;
    QLineEdit *const mServerUrl;
    QLineEdit *const mAccountName;
    QLineEdit *const mUserName;
    PasswordLineEditWidget *const mPasswordLineEditWidget;
    QPushButton *const mRegisterAccount;
};
