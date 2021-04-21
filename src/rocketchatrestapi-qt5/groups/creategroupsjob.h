/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include "createroominfo.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT CreateGroupsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CreateGroupsJob(QObject *parent = nullptr);
    ~CreateGroupsJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT CreateRoomInfo createGroupsInfo() const;
    void setCreateGroupsInfo(const CreateRoomInfo &createGroupsInfo);

Q_SIGNALS:
    void createGroupsDone(const QJsonObject &reply);

private:
    Q_DISABLE_COPY(CreateGroupsJob)
    void slotCreateGroupsFinished();
    CreateRoomInfo mCreateGroupInfo;
    // {"name":"teams-test-room2","members":[],"readOnly":false,"extraData":{"description":"","broadcast":false,"encrypted":false,"teamId":"607fb34ee8da65ad4f476487"}
    // TODO add extraData
};
}
