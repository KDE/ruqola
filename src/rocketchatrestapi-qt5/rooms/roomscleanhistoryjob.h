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

#ifndef ROOMSCLEANHISTORYJOB_H
#define ROOMSCLEANHISTORYJOB_H

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QDateTime>
#include <QDebug>
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsCleanHistoryJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT CleanHistoryInfo {
        Q_REQUIRED_RESULT bool isValid() const;
        QDateTime latest; //required
        QDateTime oldest; //required
        QString roomId; //required
        QStringList users;
        bool inclusive = false;
        bool excludePinned = false;
        bool filesOnly = false;
        bool ignoreThreads = false;
    };
    explicit RoomsCleanHistoryJob(QObject *parent = nullptr);
    ~RoomsCleanHistoryJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT CleanHistoryInfo cleanHistoryInfo() const;
    void setCleanHistoryInfo(const CleanHistoryInfo &cleanHistoryInfo);

Q_SIGNALS:
    void cleanHistoryDone();

private:
    Q_DISABLE_COPY(RoomsCleanHistoryJob)
    void slotCleanHistoryFinished();
    CleanHistoryInfo mCleanHistoryInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QDebug operator <<(QDebug d, const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &t);
#endif // ROOMSCLEANHISTORYJOB_H
