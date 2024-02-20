/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsNameExistsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RoomsNameExistsJob(QObject *parent = nullptr);
    ~RoomsNameExistsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] const QString &roomName() const;
    void setRoomName(const QString &newRoomId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void roomNameExistsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(RoomsNameExistsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomName;
};
}
