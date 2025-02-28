/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewscommandjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
PreviewsCommandJob::PreviewsCommandJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PreviewsCommandJob::~PreviewsCommandJob() = default;

bool PreviewsCommandJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("PreviewsCommandJob::start");
    submitPostRequest(json());

    return true;
}

void PreviewsCommandJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("PreviewsCommandJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT previewsCommandDone(replyObject);
    } else {
        Q_EMIT previewsCommandFailed(mPreviewsCommandInfo);
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("PreviewsCommandJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

PreviewsCommandJob::PreviewsCommandInfo PreviewsCommandJob::previewsCommandInfo() const
{
    return mPreviewsCommandInfo;
}

void PreviewsCommandJob::setPreviewsCommandInfo(const PreviewsCommandInfo &runCommandInfo)
{
    mPreviewsCommandInfo = runCommandInfo;
}

PreviewsCommandJob::PreviewsCommandInfo PreviewsCommandJob::parseString(const QString &str, const QByteArray &roomId)
{
    PreviewsCommandJob::PreviewsCommandInfo info;
    if (str.length() > 1) {
        const QString newStr = str.mid(1);
        QStringList lst = newStr.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        const int numberElement = lst.count();
        info.commandName = lst.takeAt(0);
        info.roomId = QLatin1StringView(roomId);
        if (numberElement > 1) {
            info.params = lst.join(QLatin1Char(' '));
        }
    }
    return info;
}

bool PreviewsCommandJob::requireHttpAuthentication() const
{
    return true;
}

bool PreviewsCommandJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mPreviewsCommandInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "PreviewsCommandJob: RoomId and CommandName are empty";
        return false;
    }
    return true;
}

QNetworkRequest PreviewsCommandJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CommandsPreview);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument PreviewsCommandJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["command"_L1] = mPreviewsCommandInfo.commandName;
    jsonObj["roomId"_L1] = mPreviewsCommandInfo.roomId;

    if (!mPreviewsCommandInfo.params.isEmpty()) {
        jsonObj["params"_L1] = mPreviewsCommandInfo.params;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool PreviewsCommandJob::PreviewsCommandInfo::isValid() const
{
    return !commandName.isEmpty() && !roomId.isEmpty();
}

QDebug operator<<(QDebug d, const PreviewsCommandJob::PreviewsCommandInfo &t)
{
    d.space() << "commandName" << t.commandName;
    d.space() << "roomId" << t.roomId;
    d.space() << "params" << t.params;
    return d;
}

#include "moc_previewscommandjob.cpp"
