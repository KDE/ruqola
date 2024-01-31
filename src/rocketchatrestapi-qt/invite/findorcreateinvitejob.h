/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT FindOrCreateInviteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT InviteUsersInfo {
        int maxUses = -1;
        QUrl url;
        QString roomId;
        QString userId;
        QString expireDateTime;
        [[nodiscard]] bool operator==(const InviteUsersInfo &other) const;
    };
    explicit FindOrCreateInviteJob(QObject *parent = nullptr);
    ~FindOrCreateInviteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] int maxUses() const;
    void setMaxUses(int maxUses);

    [[nodiscard]] int numberOfDays() const;
    void setNumberOfDays(int numberOfDays);

    [[nodiscard]] FindOrCreateInviteJob::InviteUsersInfo parseResult(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

Q_SIGNALS:
    void findOrCreateInviteDone(const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &info);

private:
    Q_DISABLE_COPY(FindOrCreateInviteJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    int mMaxUses = 20;
    int mNumberOfDays = 0;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &t);
