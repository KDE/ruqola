/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createupdateuserinfo.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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
    d.space() << "roles " << t.mRoles;
    d.space() << "mUserId " << t.mUserId;
    d.space() << "mEmail " << t.mEmail;
    d.space() << "mName " << t.mName;
    d.space() << "mUserName " << t.mUserName;
    d.space() << "mBio " << t.mBio;
    // d.space() << "mPassword " << t.mPassword;
    d.space() << "mStatusText " << t.mStatusText;
    d.space() << "mNickName " << t.mNickName;
    d.space() << "mRequirePasswordChange " << t.mRequirePasswordChange;
    d.space() << "mAssignRandomPassword " << t.mSetRandomPassword;
    d.space() << "mSendWelcomeEmail " << t.mSendWelcomeEmail;
    return d;
}

QJsonDocument CreateUpdateUserInfo::json() const
{
    // TODO add update support
    QJsonObject userInfoObj;
    if (!mEmail.isEmpty()) {
        userInfoObj["email"_L1] = mEmail;
    }
    if (!mName.isEmpty()) {
        userInfoObj["name"_L1] = mName;
    }
    if (!mUserName.isEmpty()) {
        userInfoObj["username"_L1] = mUserName;
    }
    if (!mStatusText.isEmpty()) {
        userInfoObj["statusText"_L1] = mStatusText;
    }
    if (!mBio.isEmpty()) {
        userInfoObj["bio"_L1] = mBio;
    }
    if (!mNickName.isEmpty()) {
        userInfoObj["nickname"_L1] = mNickName;
    }
    if (!mRoles.isEmpty()) {
        userInfoObj["roles"_L1] = QJsonArray::fromStringList(mRoles);
    }

    userInfoObj["password"_L1] = mSetRandomPassword ? QString() : mPassword;

    userInfoObj["requirePasswordChange"_L1] = mRequirePasswordChange;
    userInfoObj["sendWelcomeEmail"_L1] = mSendWelcomeEmail;
    userInfoObj["setRandomPassword"_L1] = mSetRandomPassword;
    userInfoObj["verified"_L1] = mVerified;
    if (mTypeInfo == Update) {
        QJsonObject dataObj;
        dataObj["data"_L1] = userInfoObj;
        if (!mUserId.isEmpty()) {
            dataObj["userId"_L1] = mUserId;
        }
        const QJsonDocument postData = QJsonDocument(dataObj);
        return postData;
    }
    const QJsonDocument postData = QJsonDocument(userInfoObj);
    return postData;
}
