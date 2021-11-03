/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include <QWidget>

#include "roles/roleinfo.h"

#include "users/userscreatejob.h"

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QCheckBox;
class User;
class KPasswordLineEdit;
class RolesComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorAddUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorAddUserWidget(QWidget *parent = nullptr);
    ~AdministratorAddUserWidget() override;

    Q_REQUIRED_RESULT RocketChatRestApi::CreateUpdateUserInfo createInfo() const;

    void setUser(const User &user);

    Q_REQUIRED_RESULT RocketChatRestApi::UpdateUserInfo updateInfo() const;
    void setRoleInfo(const QVector<RoleInfo> &info);
Q_SIGNALS:
    void updateButtonOk(bool b);

private:
    void slotUpdateOkButton();
    QLineEdit *const mName;
    QLineEdit *const mUserName;
    QLineEdit *const mEmail;
    QLineEdit *const mStatusText;

    QCheckBox *const mJoinDefaultChannels;
    QCheckBox *const mSendWelcomeEmails;
    KPasswordLineEdit *const mPasswordLineEdit;
    RolesComboBox *const mRolesComboBox;
};
