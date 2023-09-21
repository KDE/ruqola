/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "users/registeruserjob.h"
#include <QWidget>
class QLineEdit;
class KPasswordLineEdit;
class QPushButton;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RegisterUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterUserWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~RegisterUserWidget() override;
    [[nodiscard]] RocketChatRestApi::RegisterUserJob::RegisterUserInfo registerUserInfo() const;

Q_SIGNALS:
    void updateOkButton(bool b);

private:
    void slotRegisterNewUser();
    void slotUpdateRegisterButton();
    QPushButton *const mRegisterButton;
    QLineEdit *const mUserName;
    QLineEdit *const mEmail;
    KPasswordLineEdit *const mPasswordLineEdit;
    KPasswordLineEdit *const mConfirmPasswordLineEdit;
    RocketChatAccount *const mRocketchatAccount;
};
