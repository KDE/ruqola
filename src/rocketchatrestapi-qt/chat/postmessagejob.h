/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT PostMessageJob : public RestApiAbstractJob
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

    [[nodiscard]] QList<QByteArray> roomIds() const;
    void setRoomIds(const QList<QByteArray> &roomId);

    [[nodiscard]] QJsonDocument json() const;

Q_SIGNALS:
    void postMessageDone(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString generateErrorMessage(const QString &errorStr) const override;
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QList<QByteArray> mRoomIds;
    QString mText;
};
}
