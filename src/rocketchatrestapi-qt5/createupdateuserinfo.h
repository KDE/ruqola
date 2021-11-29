/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include <QDebug>
#include <QJsonDocument>
#include <QString>
#include <QStringList>
namespace RocketChatRestApi
{
struct LIBROCKETCHATRESTAPI_QT5_EXPORT CreateUpdateUserInfo {
    enum InfoType {
        Create = 0,
        Update,
    };

    QStringList mRoles;
    QString mUserId;
    QString mEmail;
    QString mName;
    QString mUserName;
    QString mPassword;
    QString mStatusText;
    QString mBio;
    QString mNickName;
    InfoType mTypeInfo = Create;
    bool mJoinDefaultChannels = false;
    bool mRequirePasswordChange = false;
    bool mSendWelcomeEmail = false;
    bool mSetRandomPassword = false;
    bool mVerified = false;
    Q_REQUIRED_RESULT bool isValid() const;
    Q_REQUIRED_RESULT QJsonDocument json() const;
};

struct LIBROCKETCHATRESTAPI_QT5_EXPORT UpdateUserInfo : public CreateUpdateUserInfo {
    UpdateUserInfo()
        : CreateUpdateUserInfo()
    {
        mTypeInfo = Update;
    }
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::CreateUpdateUserInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::CreateUpdateUserInfo &t);
