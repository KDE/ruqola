/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QDateTime>
#include <QDebug>
namespace RocketChatRestApi
{
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
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    void createJsonForFile(QJsonObject &jsonObj) const;
    void createJsonForEmail(QJsonObject &jsonObj) const;
    RoomsExportInfo mRoomExportInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat)
Q_DECLARE_TYPEINFO(RocketChatRestApi::RoomsExportJob::RoomsExportInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &t);
