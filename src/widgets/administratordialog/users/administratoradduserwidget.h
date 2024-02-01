/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "users/userscreatejob.h"

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QCheckBox;
class User;
class KPasswordLineEdit;
class RolesComboBox;
class QPlainTextEdit;
class RocketChatAccount;
class RoleInfo;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorAddUserWidget : public QWidget
{
    Q_OBJECT
public:
    enum UserType {
        Unknown = 0,
        Create,
        Edit,
    };

    explicit AdministratorAddUserWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorAddUserWidget() override;

    [[nodiscard]] RocketChatRestApi::CreateUpdateUserInfo createInfo() const;

    void setUser(const User &user);

    [[nodiscard]] RocketChatRestApi::UpdateUserInfo updateInfo() const;
    void setRoleInfo(const QVector<RoleInfo> &info);
Q_SIGNALS:
    void updateButtonOk(bool b);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChangeSetRandomPassword(bool checked);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateOkButton();
    UserType mUserType = UserType::Create;
    QLineEdit *const mName;
    QLineEdit *const mUserName;
    QLineEdit *const mStatusText;
    QLineEdit *const mEmail;
    QLineEdit *const mNickName;
    KPasswordLineEdit *const mPasswordLineEdit;

    QCheckBox *const mJoinDefaultChannels;
    QCheckBox *const mSendWelcomeEmails;
    QCheckBox *const mRequirePasswordChange;
    QCheckBox *const mSetRandowPassword;
    QCheckBox *const mEmailVerified;
    RolesComboBox *const mRolesComboBox;
    QPlainTextEdit *const mBioPlainTextEdit;
    RocketChatAccount *const mRocketChatAccount;
};
