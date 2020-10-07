/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef USERSCREATEJOB_H
#define USERSCREATEJOB_H

#include "restapiabstractjob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT UsersCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT CreateInfo {
        //TODO add enum ?
        QString mUserId;
        QString mEmail;
        QString mName;
        QString mUserName;
        QString mPassword;
        Q_REQUIRED_RESULT bool isValid() const;
        //TODO add more
    };
    explicit UsersCreateJob(QObject *parent = nullptr);
    ~UsersCreateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT CreateInfo createInfo() const;
    void setCreateInfo(const CreateInfo &createInfo);

Q_SIGNALS:
    void usersCreateDone();

private:
    Q_DISABLE_COPY(UsersCreateJob)
    void slotUsersCreate();
    CreateInfo mCreateInfo;
};
}
#endif // UsersCreateJob_H
