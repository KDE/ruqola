/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userrequestdatadownloadjob.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UserRequestDataDownloadJob::UserRequestDataDownloadJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UserRequestDataDownloadJob::~UserRequestDataDownloadJob() = default;

bool UserRequestDataDownloadJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool UserRequestDataDownloadJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("UserRequestDataDownloadJob start");
    return true;
}

void UserRequestDataDownloadJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UserRequestDataDownloadJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        const QString result = replyObject[QStringLiteral("result")].toString();
        Q_EMIT userRequestDataDownloadDone(result);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("UserRequestDataDownloadJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool UserRequestDataDownloadJob::fullExport() const
{
    return mFullExport;
}

void UserRequestDataDownloadJob::setFullExport(bool newFullExport)
{
    mFullExport = newFullExport;
}

QNetworkRequest UserRequestDataDownloadJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UserRequestDataDownload);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool UserRequestDataDownloadJob::requireHttpAuthentication() const
{
    return true;
}
