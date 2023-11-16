/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AdminRoomsGetRoomJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit AdminRoomsGetRoomJob(QObject *parent = nullptr);
    ~AdminRoomsGetRoomJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void adminRoomGetRoomDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(AdminRoomsGetRoomJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
