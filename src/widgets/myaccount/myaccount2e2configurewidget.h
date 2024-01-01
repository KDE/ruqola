/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QPushButton;
class RocketChatAccount;
class PasswordConfirmWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2e2ConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccount2e2ConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccount2e2ConfigureWidget() override;

    void save();
    void load();

private:
    void slotReset2E2KeyDone(const QJsonObject &replyObject);
    void slotResetE2EKey();
    QPushButton *const mResetE2eKey;
    RocketChatAccount *const mRocketChatAccount;
    PasswordConfirmWidget *const mPasswordConfirmWidget;
};
