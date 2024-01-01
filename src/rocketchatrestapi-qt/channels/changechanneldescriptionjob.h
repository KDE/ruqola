/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChangeChannelDescriptionJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeChannelDescriptionJob(QObject *parent = nullptr);
    ~ChangeChannelDescriptionJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

Q_SIGNALS:
    void changeDescriptionDone();

private:
    Q_DISABLE_COPY(ChangeChannelDescriptionJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mDescription;
    QString mRoomId;
};
}
