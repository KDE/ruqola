/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsexportjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsExportJob::RoomsExportJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsExportJob::~RoomsExportJob() = default;

bool RoomsExportJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsExportJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsExportJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsExportJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomExportDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsExportJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool RoomsExportJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsExportJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoomExportInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mRoomExportInfo: mRoomExportInfo is not valid.";
        return false;
    }

    return true;
}

QJsonDocument RoomsExportJob::json() const
{
    QJsonObject jsonObj;

    jsonObj["rid"_L1] = QLatin1StringView(mRoomExportInfo.roomId);
    switch (mRoomExportInfo.exportAs) {
    case RoomsExportInfo::ExportAs::Unknown:
        // Nothing it's a bug here.
        break;
    case RoomsExportInfo::ExportAs::File:
        jsonObj["type"_L1] = u"file"_s;
        createJsonForFile(jsonObj);
        break;
    case RoomsExportInfo::ExportAs::Email:
        jsonObj["type"_L1] = u"email"_s;
        createJsonForEmail(jsonObj);
        break;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

void RoomsExportJob::createJsonForFile(QJsonObject &jsonObj) const
{
    switch (mRoomExportInfo.fileFormat) {
    case RoomsExportInfo::FileFormat::Unknown:
        // It's a bug
        break;
    case RoomsExportInfo::FileFormat::Html:
        jsonObj["format"_L1] = u"html"_s;
        break;
    case RoomsExportInfo::FileFormat::Json:
        jsonObj["format"_L1] = u"json"_s;
        break;
    }
    jsonObj["dateTo"_L1] = mRoomExportInfo.dateTo.date().toString(Qt::ISODate);
    jsonObj["dateFrom"_L1] = mRoomExportInfo.dateFrom.date().toString(Qt::ISODate);
}

void RoomsExportJob::createJsonForEmail(QJsonObject &jsonObj) const
{
    jsonObj["toUsers"_L1] = QJsonArray::fromStringList(mRoomExportInfo.toUsers);
    jsonObj["toEmails"_L1] = QJsonArray::fromStringList(mRoomExportInfo.toEmails);
    jsonObj["subject"_L1] = mRoomExportInfo.subject;
    jsonObj["messages"_L1] = mRoomExportInfo.messages;
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
    // qDebug() << " url ************* " << url;
    return request;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &t)
{
    d.space() << "roomId " << t.roomId;
    d.space() << "toUsers " << t.toUsers;
    d.space() << "toEmails " << t.toEmails;
    d.space() << "subject " << t.subject;
    d.space() << "messages " << t.messages;
    d.space() << "dateFrom " << t.dateFrom;
    d.space() << "dateTo " << t.dateTo;
    d.space() << "fileFormat " << static_cast<int>(t.fileFormat);
    d.space() << "exportAs " << static_cast<int>(t.exportAs);
    return d;
}

bool RoomsExportJob::RoomsExportInfo::isValid() const
{
    // Check from exportas type
    return fileFormat != RoomsExportInfo::FileFormat::Unknown && exportAs != RoomsExportInfo::ExportAs::Unknown && !roomId.isEmpty();
}

#include "moc_roomsexportjob.cpp"
