/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "ddpapi/ddpclient.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QPushButton;
class QLabel;
class KBusyIndicatorWidget;
class PasswordLineEditWidget;
class TwoAuthenticationPasswordWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginWidget(QWidget *parent = nullptr);
    ~RuqolaLoginWidget() override;

    void setRocketChatAccount(RocketChatAccount *account);
    void setLoginStatus(AuthenticationManager::LoginStatus status);
    void showError(const QString &text);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void changeWidgetStatus(bool enabled);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLogin();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetPasswordRequested(const QString &email);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateLoginButton();

    QLineEdit *const mAccountName;
    QLineEdit *const mServerUrl;
    QLineEdit *const mUserName;
    PasswordLineEditWidget *const mPasswordLineEditWidget;

    QMetaObject::Connection mUpdatePasswordConnection;
    QPushButton *const mLoginButton;
    KBusyIndicatorWidget *const mBusyIndicatorWidget;
    QLabel *const mFailedError;
    TwoAuthenticationPasswordWidget *const mTwoFactorAuthenticationPasswordLineEdit;
    QWidget *const mTwoFactorAuthenticationWidget;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
