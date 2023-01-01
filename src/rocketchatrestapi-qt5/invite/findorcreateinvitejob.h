/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT FindOrCreateInviteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT InviteUsersInfo {
        int maxUses = -1;
        QUrl url;
        QString roomId;
        QString userId;
        QString expireDateTime;
        Q_REQUIRED_RESULT bool operator==(const InviteUsersInfo &other) const;
    };
    explicit FindOrCreateInviteJob(QObject *parent = nullptr);
    ~FindOrCreateInviteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT int maxUses() const;
    void setMaxUses(int maxUses);

    Q_REQUIRED_RESULT int numberOfDays() const;
    void setNumberOfDays(int numberOfDays);

    Q_REQUIRED_RESULT FindOrCreateInviteJob::InviteUsersInfo parseResult(const QJsonObject &replyObject);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;

Q_SIGNALS:
    void findOrCreateInviteDone(const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &info);

private:
    Q_DISABLE_COPY(FindOrCreateInviteJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoomId;
    int mMaxUses = 20;
    int mNumberOfDays = 0;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &t);
