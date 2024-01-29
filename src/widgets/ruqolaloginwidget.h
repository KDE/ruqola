/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ddpapi/ddpauthenticationmanager.h"
#include "ddpapi/ddpclient.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QPushButton;
class QLabel;
class KBusyIndicatorWidget;
class AuthenticationOauthWidget;
class PasswordLineEditWidget;
class TwoAuthenticationPasswordWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginWidget(QWidget *parent = nullptr);
    ~RuqolaLoginWidget() override;

    void setRocketChatAccount(RocketChatAccount *account);
    void setLoginStatus(DDPAuthenticationManager::LoginStatus status);
    void showError(const QString &text);

private:
    void changeWidgetStatus(bool enabled);
    void slotLogin();
    void slotResetPasswordRequested(const QString &email);
    void slotUpdateLoginButton();

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
    QWidget *const mAuthenticationAccountWidget;
    AuthenticationOauthWidget *const mAuthenticationWidget;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
