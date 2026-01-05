/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QPushButton;
class QLabel;
class KBusyIndicatorWidget;
class TwoAuthenticationPasswordWidget;
class RuqolaLoginStackWidget;
class RocketChatAccount;
class RuqolaLoginButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginWidget(QWidget *parent = nullptr);
    ~RuqolaLoginWidget() override;

    void setRocketChatAccount(RocketChatAccount *account);
    void setLoginStatus(AuthenticationManager::LoginStatus status);
    void showError(const QString &text);

    void forceLoginFocus();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void changeWidgetStatus(bool enabled);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLogin();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCancelLogin();

    RuqolaLoginStackWidget *const mRuqolaLoginStackWidget;
    QMetaObject::Connection mUpdatePasswordConnection;
    RuqolaLoginButton *const mLoginButton;
    KBusyIndicatorWidget *const mBusyIndicatorWidget;
    QLabel *const mFailedError;
    TwoAuthenticationPasswordWidget *const mTwoFactorAuthenticationPasswordLineEdit;
    QWidget *const mTwoFactorAuthenticationWidget;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
