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

#include "roomsexportjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoomsExportJob::RoomsExportJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsExportJob::~RoomsExportJob()
{
}

bool RoomsExportJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsExportJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RoomsExportJob::slotRoomExportFinished);
    return true;
}

void RoomsExportJob::slotRoomExportFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoomsExportJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT roomExportDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoomsExportJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool RoomsExportJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsExportJob::canStart() const
{
    if (!mRoomExportInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mRoomExportInfo: mRoomExportInfo is not valid.";
        return false;
    }

    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument RoomsExportJob::json() const
{
    QJsonObject jsonObj;

    jsonObj[QLatin1String("rid")] = mRoomExportInfo.roomId;
    switch (mRoomExportInfo.exportAs) {
    case RoomsExportInfo::ExportAs::Unknown:
        //Nothing it's a bug here.
        break;
    case RoomsExportInfo::ExportAs::File:
        createJsonForFile(jsonObj);
        break;
    case RoomsExportInfo::ExportAs::Email:
        createJsonForEmail(jsonObj);
        break;
    }
    //qDebug() << " postData**************** " << postData;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

void RoomsExportJob::createJsonForFile(QJsonObject &jsonObj) const
{
    switch (mRoomExportInfo.fileFormat) {
    case RoomsExportInfo::FileFormat::Unknown:
        //It's a bug
        break;
    case RoomsExportInfo::FileFormat::Html:
        jsonObj[QLatin1String("rid")] = QStringLiteral("html");
        break;
    case RoomsExportInfo::FileFormat::Json:
        jsonObj[QLatin1String("rid")] = QStringLiteral("json");
        break;
    }
    //TODO dateFrom/dateTo
}

void RoomsExportJob::createJsonForEmail(QJsonObject &jsonObj) const
{
    jsonObj[QLatin1String("toUsers")] = QJsonArray::fromStringList(mRoomExportInfo.toUsers);
    jsonObj[QLatin1String("toEmails")] = QJsonArray::fromStringList(mRoomExportInfo.toEmails);
    jsonObj[QLatin1String("subject")] = mRoomExportInfo.subject;
    jsonObj[QLatin1String("messages")] = mRoomExportInfo.messages;
}

RoomsExportJob::RoomsExportInfo RoomsExportJob::roomExportInfo() const
{
    return mRoomExportInfo;
}

void RoomsExportJob::setRoomExportInfo(const RoomsExportJob::RoomsExportInfo &roomExportInfo)
{
    mRoomExportInfo = roomExportInfo;
}

QNetworkRequest RoomsExportJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsExport);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QDebug operator <<(QDebug d, const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &t)
{
    d << "roomId " << t.roomId;
    d << "toUsers " << t.toUsers;
    d << "toEmails " << t.toEmails;
    d << "subject " << t.subject;
    d << "messages " << t.messages;
    d << "dateFrom " << t.dateFrom;
    d << "dateTo " << t.dateTo;
    d << "fileFormat " << static_cast<int>(t.fileFormat);
    d << "exportAs " << static_cast<int>(t.exportAs);
    return d;
}

bool RoomsExportJob::RoomsExportInfo::isValid() const
{
    //Check from exportas type
    return fileFormat != RoomsExportInfo::FileFormat::Unknown
            && exportAs != RoomsExportInfo::ExportAs::Unknown
            && !roomId.isEmpty();
}
