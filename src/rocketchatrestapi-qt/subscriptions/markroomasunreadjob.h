/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT MarkRoomAsUnReadJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // Since 0.65
    explicit MarkRoomAsUnReadJob(QObject *parent = nullptr);
    ~MarkRoomAsUnReadJob() override;

    enum MarkAsUnReadObject { Unknown = 0, FromMessage, Room };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] MarkAsUnReadObject unReadObject() const;
    void setUnReadObject(MarkAsUnReadObject type);

    [[nodiscard]] QString objectId() const;
    void setObjectId(const QString &objectId);

Q_SIGNALS:
    void markAsUnReadDone();

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(MarkRoomAsUnReadJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mObjectId;
    MarkAsUnReadObject mUnReadObject = Unknown;
};
}
