/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "roles/roleinfo.h"
#include <QVector>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLabel;
class User;
class QFormLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectChannelInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectChannelInfoWidget(QWidget *parent = nullptr);
    ~DirectChannelInfoWidget() override;

    void setUserName(const QString &userName);

    void setRoles(const QVector<RoleInfo> &newRoles);

private:
    void slotUserInfoDone(const QJsonObject &obj);
    void fetchUserInfo(const QString &userName);
    void setUser(const User &user);
    void hideWidget(QLabel *label);
    QVector<RoleInfo> mListRoleInfos;
    QLabel *const mName;
    QLabel *const mUserName;
    QLabel *const mCustomStatus;
    QLabel *const mStatus;
    QLabel *const mTimeZone;
    QLabel *const mAvatar;
    QLabel *const mRoles;
    QLabel *const mCreateAt;
    QLabel *const mLastLogin;
    QLabel *const mEmailsInfo;
    QFormLayout *const mMainLayout;
};

