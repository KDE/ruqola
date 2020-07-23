/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef RUQOLALOGINWIDGET_H
#define RUQOLALOGINWIDGET_H

#include <QWidget>
#include <ddpapi/ddpclient.h>
#include <ddpapi/ddpauthenticationmanager.h>
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QPushButton;
class KPasswordLineEdit;
class QLabel;
class KBusyIndicatorWidget;
class AuthenticationComboBox;
class PasswordLineEditWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginWidget(QWidget *parent = nullptr);
    ~RuqolaLoginWidget() override;

    void initialize();
    void setLoginStatus(DDPAuthenticationManager::LoginStatus status);
    void showError(const QString &text);

private:
    void changeWidgetStatus(bool enabled);
    void slotLogin();
    void slotSendNewEmailCode();
    void slotResetPasswordRequested(const QString &email);

    QLabel *mAccountName = nullptr;
    QLineEdit *mServerName = nullptr;
    QLineEdit *mUserName = nullptr;
    PasswordLineEditWidget *mPasswordLineEditWidget = nullptr;
    QPushButton *mLoginButton = nullptr;
    KBusyIndicatorWidget *mBusyIndicatorWidget = nullptr;
    QLabel *mFailedError = nullptr;
    KPasswordLineEdit *mTwoFactorAuthenticationPasswordLineEdit = nullptr;
    QWidget *mAuthenticationWidget = nullptr;
    QWidget *mAuthenticationAccountWidget = nullptr;
    AuthenticationComboBox *mAuthenticationCombobox = nullptr;
};

#endif // RUQOLALOGINWIDGET_H
