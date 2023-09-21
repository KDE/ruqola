/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class TwoAuthenticationPasswordWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AskTwoAuthenticationPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AskTwoAuthenticationPasswordWidget(QWidget *parent = nullptr);
    ~AskTwoAuthenticationPasswordWidget() override;
    [[nodiscard]] QString code() const;
    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);
Q_SIGNALS:
    void updateButtonOk(bool state);

private:
    TwoAuthenticationPasswordWidget *const mTwoFactorAuthenticationPasswordLineEdit;
};
