/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QPushButton;
class PasswordLineEditWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AuthenticationLoginWidget : public QWidget
{
    Q_OBJECT
public:
    enum class AuthenticationLoginType {
        Unknown = 0,
        Create,
        Modify,
        Login,
    };

    explicit AuthenticationLoginWidget(QWidget *parent = nullptr);
    ~AuthenticationLoginWidget() override;

    void setExistingAccountName(const QStringList &lst);

    void setAccountInfo(const AccountManager::AccountManagerInfo &info);
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo();

    void setAuthenticationLoginType(AuthenticationLoginType type);

    void changeAuthenticationWidgetStatus(bool enabled);
Q_SIGNALS:
    void settingsIsValid(bool enabled);
    void returnPressed();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetPasswordRequested(const QString &email);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegisterUserDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoginSettingsChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegisterAccount();
    QStringList mNames;
    AccountManager::AccountManagerInfo mAccountInfo;
    QLineEdit *const mServerUrl;
    QLineEdit *const mAccountName;
    QLineEdit *const mUserName;
    PasswordLineEditWidget *const mPasswordLineEditWidget;
    QPushButton *const mRegisterAccount;
};
