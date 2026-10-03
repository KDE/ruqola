/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChangeChannelTopicJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeChannelTopicJob(QObject *parent = nullptr);
    ~ChangeChannelTopicJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString topic() const;
    void setTopic(const QString &topic);
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void changeTopicDone();

protected:
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

private:
    QString mTopic;
    QString mRoomId;
};
}
