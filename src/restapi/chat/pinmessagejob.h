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

#ifndef PINMESSAGEJOB_H
#define PINMESSAGEJOB_H

#include "restapiabstractjob.h"
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT PinMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PinMessageJob(QObject *parent = nullptr);
    ~PinMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT bool pinMessage() const;
    void setPinMessage(bool pinMessage);

    Q_REQUIRED_RESULT QJsonDocument json() const;
Q_SIGNALS:
    void pinMessageDone();
    void unPinMessageDone();

private:
    Q_DISABLE_COPY(PinMessageJob)
    void slotPinMessage();
    QString mMessageId;
    bool mPinMessage = true;
};

#endif // PINMESSAGEJOB_H
