/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT IgnoreUserJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit IgnoreUserJob(QObject *parent = nullptr);
    ~IgnoreUserJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);

    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QString ignoreUserId() const;
    void setIgnoreUserId(const QString &ignoreUserId);

    [[nodiscard]] bool ignore() const;
    void setIgnore(bool ignore);

Q_SIGNALS:
    void ignoreUserDone(const QJsonObject &obj, const QByteArray &roomId);

private:
    Q_DISABLE_COPY(IgnoreUserJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mRoomId;
    QString mIgnoreUserId;
    bool mIgnore = true;
};
}
