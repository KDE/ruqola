/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GroupsInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GroupsInfoJob(QObject *parent = nullptr);
    ~GroupsInfoJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void channelInfoDone(const QJsonObject &replyObjectj, const QString &roomId);

private:
    Q_DISABLE_COPY(GroupsInfoJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
