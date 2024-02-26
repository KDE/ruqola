/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ddpapi/ddpauthenticationmanager.h"
#include <QStackedWidget>

class RuqolaLoginWidget;
class RocketChatAccount;
class RuqolaLoginStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginStackWidget(QWidget *parent = nullptr);
    ~RuqolaLoginStackWidget() override;

    void setRocketChatAccount(RocketChatAccount *account);
    void setLoginStatus(DDPAuthenticationManager::LoginStatus status);
    void showError(const QString &text);

private:
    RuqolaLoginWidget *const mRuqolaLoginWidget;
};
