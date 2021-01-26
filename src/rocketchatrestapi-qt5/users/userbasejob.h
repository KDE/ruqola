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

#ifndef USERBASEJOB_H
#define USERBASEJOB_H

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UserBaseJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class UserInfoType {
        Unknown,
        UserId,
        UserName,
    };

    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UserInfo {
        QString userIdentifier;
        UserBaseJob::UserInfoType userInfoType = UserBaseJob::UserInfoType::Unknown;
    };
    explicit UserBaseJob(QObject *parent = nullptr);
    ~UserBaseJob() override;

    Q_REQUIRED_RESULT UserInfo userInfo() const;
    void setUserInfo(const UserInfo &userInfo);

    Q_REQUIRED_RESULT bool hasUserIdentifier() const;

    Q_REQUIRED_RESULT QString identifier() const;

protected:
    void addQueryUrl(QUrl &url) const;
    void generateJson(QJsonObject &jsonObj) const;
    UserInfo mUserInfo;
};
}

#endif // USERBASEJOB_H
