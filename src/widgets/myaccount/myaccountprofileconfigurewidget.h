/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "ownuser/ownuser.h"
#include <QWidget>
class QLineEdit;
class QPushButton;
class PasswordConfirmWidget;
class MyAccountProfileConfigureAvatarWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountProfileConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountProfileConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountProfileConfigureWidget() override;
    void load();
    void save();
    void initialize();

private:
    void slotLogoutFromOtherLocation();
    void slotDeleteMyAccount();
    QLineEdit *const mEmail;
    QLineEdit *const mName;
    QLineEdit *const mUserName;
    QLineEdit *const mNickName;
    QLineEdit *const mStatusText;
    QPushButton *const mDeleteMyAccount;
    QPushButton *const mLogoutFromOtherLocation;
    PasswordConfirmWidget *const mPasswordConfirmWidget;
    MyAccountProfileConfigureAvatarWidget *const mConfigureAvatarWidget;
    RocketChatAccount *const mRocketChatAccount;
    OwnUser mOwnUser;
};
