/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
        if (mSetRandomPassword) {
            return valid;
        }
        return valid && !mPassword.isEmpty();
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
        userInfoObj[QLatin1StringView("email")] = mEmail;
    }
    if (!mName.isEmpty()) {
        userInfoObj[QLatin1StringView("name")] = mName;
    }
    if (!mUserName.isEmpty()) {
        userInfoObj[QLatin1StringView("username")] = mUserName;
    }
    if (!mStatusText.isEmpty()) {
        userInfoObj[QLatin1StringView("statusText")] = mStatusText;
    }
    if (!mBio.isEmpty()) {
        userInfoObj[QLatin1StringView("bio")] = mBio;
    }
    if (!mNickName.isEmpty()) {
        userInfoObj[QLatin1StringView("nickname")] = mNickName;
    }
    if (!mRoles.isEmpty()) {
        userInfoObj[QLatin1StringView("roles")] = QJsonArray::fromStringList(mRoles);
    }

    userInfoObj[QLatin1StringView("password")] = mSetRandomPassword ? QString() : mPassword;

    userInfoObj[QLatin1StringView("requirePasswordChange")] = mRequirePasswordChange;
    userInfoObj[QLatin1StringView("sendWelcomeEmail")] = mSendWelcomeEmail;
    userInfoObj[QLatin1StringView("setRandomPassword")] = mSetRandomPassword;
    userInfoObj[QLatin1StringView("verified")] = mVerified;
    userInfoObj[QLatin1StringView("joinDefaultChannels")] = mJoinDefaultChannels;
    if (mTypeInfo == Update) {
        QJsonObject dataObj;
        dataObj[QLatin1StringView("data")] = userInfoObj;
        dataObj[QLatin1StringView("userId")] = mUserId;
        const QJsonDocument postData = QJsonDocument(dataObj);
        return postData;
    }
    const QJsonDocument postData = QJsonDocument(userInfoObj);
    return postData;
}
