/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef FINDORCREATEINVITEJOB_H
#define FINDORCREATEINVITEJOB_H

#include "restapiabstractjob.h"
#include "librocketchatrestapi-qt5_export.h"

namespace RocketChatRestApi {
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
    void slotFindOrCreateInviteFinished();
    QString mRoomId;
    int mMaxUses = 20;
    int mNumberOfDays = 0;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QDebug operator <<(QDebug d, const RocketChatRestApi::FindOrCreateInviteJob::InviteUsersInfo &t);
#endif // FINDORCREATEINVITEJOB_H
