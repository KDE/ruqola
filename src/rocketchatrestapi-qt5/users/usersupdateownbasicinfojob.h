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

#ifndef USERSUPDATEOWNBASICINFO_H
#define USERSUPDATEOWNBASICINFO_H

#include "restapiabstractjob.h"
#include "librocketchatrestapi-qt5_export.h"
#include <QDebug>

namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_EXPORT UsersUpdateOwnBasicInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UpdateOwnBasicInfo {
        enum class BasicInfoType {
            Unknown = 0,
            Email = 1,
            Name = 2,
            UserName = 4,
            NickName = 8,
            StatusText = 16,
            Password = 32
        };
        Q_DECLARE_FLAGS(BasicInfoTypes, BasicInfoType)

        BasicInfoTypes type = {};
        QString email;
        QString name;
        QString userName;
        QString nickName;
        QString statusText;
        QByteArray currentPassword;
        QString newPassword;
        Q_REQUIRED_RESULT bool isValid() const;
        //Add custom field ?
    };

    explicit UsersUpdateOwnBasicInfoJob(QObject *parent = nullptr);
    ~UsersUpdateOwnBasicInfoJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT UpdateOwnBasicInfo updateOwnBasicInfo() const;
    void setUpdateOwnBasicInfo(const UpdateOwnBasicInfo &updateOwnBasicInfo);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str) override;

Q_SIGNALS:
    void updateOwnBasicInfoDone();
    void passwordChanged();

private:
    Q_DISABLE_COPY(UsersUpdateOwnBasicInfoJob)
    void slotUpdateOwnBasicInfo();
    UpdateOwnBasicInfo mUpdateOwnBasicInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator <<(QDebug d, const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &t);
#endif // UsersUpdateOwnBasicInfo_H
