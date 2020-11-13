/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef DELETEMESSAGEJOB_H
#define DELETEMESSAGEJOB_H
#include "restapiabstractjob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT DeleteMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DeleteMessageJob(QObject *parent = nullptr);
    ~DeleteMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &msgId);
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);
Q_SIGNALS:
    void deleteMessageDone();

protected:
    Q_REQUIRED_RESULT QString jobName() const override;
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(DeleteMessageJob)
    void slotDeleteMessageFinished();
    QString mRoomId;
    QString mMessageId;
};
}
#endif // DELETEMESSAGEJOB_H
