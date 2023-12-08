/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class RocketChatAccount;
class QVBoxLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AuthenticationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthenticationWidget(QWidget *parent = nullptr);
    ~AuthenticationWidget() override;

    void switchRocketChatAccount(RocketChatAccount *account);

Q_SIGNALS:
    void authentication(AuthenticationManager::OauthType type);

private:
    QVBoxLayout *const mMainLayout;
};
