/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class KPasswordLineEdit;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TwoAuthenticationPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TwoAuthenticationPasswordWidget(QWidget *parent = nullptr);
    ~TwoAuthenticationPasswordWidget() override;
    Q_REQUIRED_RESULT QString code() const;

    void clear();

    Q_REQUIRED_RESULT RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

Q_SIGNALS:
    void updateButtonOk(bool state);

private:
    void slotSendNewEmailCode();
    KPasswordLineEdit *const mTwoFactorAuthenticationPasswordLineEdit;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
