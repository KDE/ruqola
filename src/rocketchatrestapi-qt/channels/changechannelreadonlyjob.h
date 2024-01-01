/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChangeChannelReadonlyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeChannelReadonlyJob(QObject *parent = nullptr);
    ~ChangeChannelReadonlyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] bool readOnly() const;
    void setReadOnly(bool readOnly);

Q_SIGNALS:
    void changeReadonlyDone();

private:
    Q_DISABLE_COPY(ChangeChannelReadonlyJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    bool mReadOnly = false;
};
}
