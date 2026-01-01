/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
class QLabel;
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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLogoutFromOtherLocation();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeleteMyAccount();
    MyAccountProfileConfigureAvatarWidget *const mConfigureAvatarWidget;
    QLineEdit *const mName;
    QLineEdit *const mUserName;
    QLineEdit *const mEmail;
    QLabel *const mEmailInfo;
    QLabel *const mUserNameInfo;
    QLineEdit *const mNickName;
    QLineEdit *const mStatusText;
    QLabel *const mStatusTextInfo;
    QPushButton *const mDeleteMyAccount;
    QPushButton *const mLogoutFromOtherLocation;
    PasswordConfirmWidget *const mPasswordConfirmWidget;
    RocketChatAccount *const mRocketChatAccount;
    OwnUser mOwnUser;
};
