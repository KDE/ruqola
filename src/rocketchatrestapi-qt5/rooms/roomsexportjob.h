/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef ROOMSEXPORTJOB_H
#define ROOMSEXPORTJOB_H

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QDateTime>
#include <QDebug>
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsExportJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsExportInfo {
        enum class FileFormat {
            Unknown,
            Html,
            Json,
        };
        enum class ExportAs {
            Unknown,
            File,
            Email,
        };

        Q_REQUIRED_RESULT bool isValid() const;
        QString roomId;
        QStringList toUsers;
        QStringList toEmails;
        QString subject;
        QString messages;
        FileFormat fileFormat = FileFormat::Unknown;
        ExportAs exportAs = ExportAs::Unknown;
        QDateTime dateFrom;
        QDateTime dateTo;
    };
    explicit RoomsExportJob(QObject *parent = nullptr);
    ~RoomsExportJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT RoomsExportInfo roomExportInfo() const;
    void setRoomExportInfo(const RoomsExportInfo &roomExportInfo);

Q_SIGNALS:
    void roomExportDone();

private:
    Q_DISABLE_COPY(RoomsExportJob)
    void slotRoomExportFinished();
    RoomsExportInfo mRoomExportInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::RoomsExportJob::RoomsExportInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QDebug operator <<(QDebug d, const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &t);
#endif // ROOMSEXPORTJOB_H
