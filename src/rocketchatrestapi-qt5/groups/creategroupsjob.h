/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
