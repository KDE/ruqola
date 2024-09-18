/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QDateTime>
#include <QDebug>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsExportJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoomsExportInfo {
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

        [[nodiscard]] bool isValid() const;
        QByteArray roomId;
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

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] RoomsExportInfo roomExportInfo() const;
    void setRoomExportInfo(const RoomsExportInfo &roomExportInfo);

Q_SIGNALS:
    void roomExportDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void createJsonForFile(QJsonObject &jsonObj) const;
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void createJsonForEmail(QJsonObject &jsonObj) const;
    RoomsExportInfo mRoomExportInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::RoomsExportJob::RoomsExportInfo::FileFormat)
Q_DECLARE_TYPEINFO(RocketChatRestApi::RoomsExportJob::RoomsExportInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &t);
