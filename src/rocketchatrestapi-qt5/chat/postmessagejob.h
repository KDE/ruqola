/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT PostMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PostMessageJob(QObject *parent = nullptr);
    ~PostMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString text() const;
    void setText(const QString &text);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] QJsonDocument json() const;

Q_SIGNALS:
    void postMessageDone(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(PostMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mText;
    QString mRoomId;
};
}
