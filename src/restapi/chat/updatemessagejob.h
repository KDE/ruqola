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

#ifndef UPDATEMESSAGEJOB_H
#define UPDATEMESSAGEJOB_H
#include "restapiabstractjob.h"
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT UpdateMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UpdateMessageJob(QObject *parent = nullptr);
    ~UpdateMessageJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;
    bool canStart() const override;

    QNetworkRequest request() const override;

    QJsonDocument json() const;

    QString messageId() const;
    void setMessageId(const QString &msgId);

    QString roomId() const;
    void setRoomId(const QString &roomId);

    QString updatedText() const;
    void setUpdatedText(const QString &updatedText);

Q_SIGNALS:
    void updateMessageDone();

private:
    Q_DISABLE_COPY(UpdateMessageJob)
    void slotUpdateMessageFinished();
    QString mRoomId;
    QString mMessageId;
    QString mUpdatedText;
};

#endif // UPDATEMESSAGEJOB_H
