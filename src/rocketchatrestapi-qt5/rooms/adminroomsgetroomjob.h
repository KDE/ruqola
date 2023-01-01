/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT AdminRoomsGetRoomJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit AdminRoomsGetRoomJob(QObject *parent = nullptr);
    ~AdminRoomsGetRoomJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void adminRoomGetRoomDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(AdminRoomsGetRoomJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
