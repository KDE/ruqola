/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ChangeArchivationStateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeArchivationStateJob(QObject *parent = nullptr);
    ~ChangeArchivationStateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT bool archive() const;
    void setArchive(bool archive);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void changeArchivationStateDone();

private:
    Q_DISABLE_COPY(ChangeArchivationStateJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    bool mArchive = true;
};
}
