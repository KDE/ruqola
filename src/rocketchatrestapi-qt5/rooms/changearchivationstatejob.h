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

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool archive() const;
    void setArchive(bool archive);

    [[nodiscard]] QString roomId() const;
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
