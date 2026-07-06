/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletemessagefilejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
DeleteMessageFileJob::DeleteMessageFileJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DeleteMessageFileJob::~DeleteMessageFileJob() = default;

bool DeleteMessageFileJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteMessageFileJob::start");
    submitPostRequest(json());

    return true;
}

void DeleteMessageFileJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("DeleteMessageFileJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deleteMessageFileDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("DeleteMessageFileJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray DeleteMessageFileJob::fileId() const
{
    return mFileId;
}

void DeleteMessageFileJob::setFileId(const QByteArray &newFileId)
{
    mFileId = newFileId;
}

bool DeleteMessageFileJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteMessageFileJob::canStart() const
{
    if (mFileId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteMessageFileJob: mFileId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument DeleteMessageFileJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["fileId"_L1] = QLatin1StringView(mFileId);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest DeleteMessageFileJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString DeleteMessageFileJob::jobName() const
{
    return u"Delete File Message"_s;
}

#include "moc_deletemessagefilejob.cpp"
