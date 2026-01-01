/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

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

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotReset2E2KeyDone(const QJsonObject &replyObject);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetE2EKey();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyPassword();
    PasswordConfirmWidget *const mPasswordConfirmWidget;
    QPushButton *const mResetE2eKey;
    QPushButton *const mModifyE2ePassword;
    RocketChatAccount *const mRocketChatAccount;
};
