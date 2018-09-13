/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#ifndef CHANNELADDOWNERJOB_H
#define CHANNELADDOWNERJOB_H

#include "restapiabstractjob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelAddOwnerJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelAddOwnerJob(QObject *parent = nullptr);
    ~ChannelAddOwnerJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;
    bool canStart() const override;

    QNetworkRequest request() const override;

    QJsonDocument json() const;

    QString roomId() const;
    void setRoomId(const QString &roomId);

    QString addownerUserId() const;
    void setAddownerUserId(const QString &addownerUserId);

Q_SIGNALS:
    void addOwnerDone();

private:
    Q_DISABLE_COPY(ChannelAddOwnerJob)
    void slotAddOwnerFinished();
    QString mRoomId;
    QString mAddownerUserId;
};
}
#endif // CHANGECHANNELANNOUNCEMENT_H
