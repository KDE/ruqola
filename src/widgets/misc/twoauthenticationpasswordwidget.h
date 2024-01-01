/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] QString code() const;

    void clear();

    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

Q_SIGNALS:
    void updateButtonOk(bool state);

private:
    void slotSendNewEmailCode();
    KPasswordLineEdit *const mTwoFactorAuthenticationPasswordLineEdit;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
