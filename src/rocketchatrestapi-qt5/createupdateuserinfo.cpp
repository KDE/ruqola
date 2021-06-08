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

#include "createupdateuserinfo.h"

#include <QJsonArray>
#include <QJsonObject>
using namespace RocketChatRestApi;

bool CreateUpdateUserInfo::isValid() const
{
    return !mUserId.isEmpty();
}

QDebug operator<<(QDebug d, const RocketChatRestApi::CreateUpdateUserInfo &t)
{
    d << "roles " << t.mRoles;
    d << "mUserId " << t.mUserId;
    d << "mEmail " << t.mEmail;
    d << "mName " << t.mName;
    d << "mUserName " << t.mUserName;
    // d << "mPassword " << t.mPassword;
    d << "mStatusText " << t.mStatusText;
    d << "mJoinDefaultChannels " << t.mJoinDefaultChannels;
    d << "mRequirePasswordChange " << t.mRequirePasswordChange;
    d << "mAssignRandomPassword " << t.mAssignRandomPassword;
    d << "mSendWelcomeEmail " << t.mSendWelcomeEmail;
    return d;
}

QJsonDocument CreateUpdateUserInfo::json() const
{
    QJsonObject dataObj;
    if (!mEmail.isEmpty()) {
        dataObj[QLatin1String("email")] = mEmail;
    }
    if (!mName.isEmpty()) {
        dataObj[QLatin1String("name")] = mName;
    }
    if (!mPassword.isEmpty()) {
        dataObj[QLatin1String("password")] = mPassword;
    }
    if (!mUserName.isEmpty()) {
        dataObj[QLatin1String("username")] = mUserName;
    }
    if (!mStatusText.isEmpty()) {
        dataObj[QLatin1String("statusText")] = mStatusText;
    }
    if (!mRoles.isEmpty()) {
        dataObj[QLatin1String("roles")] = QJsonArray::fromStringList(mRoles);
    }
    dataObj[QLatin1String("requirePasswordChange")] = mRequirePasswordChange;
    dataObj[QLatin1String("sendWelcomeEmail")] = mSendWelcomeEmail;
    dataObj[QLatin1String("setRandomPassword")] = mSetRandomPassword;
    dataObj[QLatin1String("verified")] = mVerified;
    const QJsonDocument postData = QJsonDocument(dataObj);
    return postData;
}
