/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "ruqolaserverconfig.h"
#include "users/registeruserjob.h"
#include <QWidget>
class QLineEdit;
class PasswordConfirmWidget;
class QPushButton;
class QPlainTextEdit;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RegisterUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterUserWidget(QWidget *parent = nullptr);
    ~RegisterUserWidget() override;

    [[nodiscard]] RocketChatRestApi::RegisterUserJob::RegisterUserInfo registerUserInfo() const;

    void setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings);
    void setManuallyApproveNewUsersRequired(bool manual);
Q_SIGNALS:
    void updateOkButton(bool b);
    void registerNewAccount();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegisterNewUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateRegisterButton();
    QPushButton *const mRegisterButton;
    QLineEdit *const mUserName;
    QLineEdit *const mEmail;
    PasswordConfirmWidget *const mPasswordConfirmWidget;
    QPlainTextEdit *const mReasonTextEdit;
    QLabel *const mReasonLabel;
};
