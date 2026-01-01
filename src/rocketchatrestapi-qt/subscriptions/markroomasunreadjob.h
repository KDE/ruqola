/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT MarkRoomAsUnReadJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // Since 0.65
    explicit MarkRoomAsUnReadJob(QObject *parent = nullptr);
    ~MarkRoomAsUnReadJob() override;

    enum class MarkAsUnReadObject : uint8_t {
        Unknown = 0,
        FromMessage,
        Room
    };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] MarkAsUnReadObject unReadObject() const;
    void setUnReadObject(MarkAsUnReadObject type);

    [[nodiscard]] QByteArray objectId() const;
    void setObjectId(const QByteArray &objectId);

Q_SIGNALS:
    void markAsUnReadDone();

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mObjectId;
    MarkAsUnReadObject mUnReadObject = MarkAsUnReadObject::Unknown;
};
}
