/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createupdateuserinfo.h"

#include <QJsonArray>
#include <QJsonObject>
using namespace RocketChatRestApi;

bool CreateUpdateUserInfo::isValid() const
{
    bool valid = !mEmail.isEmpty() && !mName.isEmpty();
    if (mTypeInfo == Create) {
        return valid && !mPassword.isEmpty(); // TODO verify it
    }
    return valid && !mUserId.isEmpty(); // Need password ?
}

QDebug operator<<(QDebug d, const RocketChatRestApi::CreateUpdateUserInfo &t)
{
    d << "roles " << t.mRoles;
    d << "mUserId " << t.mUserId;
    d << "mEmail " << t.mEmail;
    d << "mName " << t.mName;
    d << "mUserName " << t.mUserName;
    d << "mBio " << t.mBio;
    // d << "mPassword " << t.mPassword;
    d << "mStatusText " << t.mStatusText;
    d << "mNickName " << t.mNickName;
    d << "mJoinDefaultChannels " << t.mJoinDefaultChannels;
    d << "mRequirePasswordChange " << t.mRequirePasswordChange;
    d << "mAssignRandomPassword " << t.mSetRandomPassword;
    d << "mSendWelcomeEmail " << t.mSendWelcomeEmail;
    return d;
}

QJsonDocument CreateUpdateUserInfo::json() const
{
    // TODO add update support
    QJsonObject userInfoObj;
    if (!mEmail.isEmpty()) {
        userInfoObj[QLatin1String("email")] = mEmail;
    }
    if (!mName.isEmpty()) {
        userInfoObj[QLatin1String("name")] = mName;
    }
    if (!mPassword.isEmpty()) {
        userInfoObj[QLatin1String("password")] = mPassword;
    }
    if (!mUserName.isEmpty()) {
        userInfoObj[QLatin1String("username")] = mUserName;
    }
    if (!mStatusText.isEmpty()) {
        userInfoObj[QLatin1String("statusText")] = mStatusText;
    }
    if (!mBio.isEmpty()) {
        userInfoObj[QLatin1String("bio")] = mBio;
    }
    if (!mNickName.isEmpty()) {
        userInfoObj[QLatin1String("nickname")] = mNickName;
    }
    if (!mRoles.isEmpty()) {
        userInfoObj[QLatin1String("roles")] = QJsonArray::fromStringList(mRoles);
    }
    userInfoObj[QLatin1String("requirePasswordChange")] = mRequirePasswordChange;
    userInfoObj[QLatin1String("sendWelcomeEmail")] = mSendWelcomeEmail;
    userInfoObj[QLatin1String("setRandomPassword")] = mSetRandomPassword;
    userInfoObj[QLatin1String("verified")] = mVerified;
    if (mTypeInfo == Update) {
        QJsonObject dataObj;
        dataObj[QLatin1String("data")] = userInfoObj;
        dataObj[QLatin1String("userId")] = mUserId;
        const QJsonDocument postData = QJsonDocument(dataObj);
        return postData;
    }
    const QJsonDocument postData = QJsonDocument(userInfoObj);
    return postData;
}
