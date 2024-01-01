/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userrequestdatadownloadjob.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
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

void UserRequestDataDownloadJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        // qDebug() << " replyObject " << replyObject;
        addLoggerInfo(QByteArrayLiteral("UserRequestDataDownloadJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        const QString result = replyObject[QLatin1String("result")].toString();
        Q_EMIT userRequestDataDownloadDone(result);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
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
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("fullExport"), mFullExport ? QStringLiteral("true") : QStringLiteral("false"));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool UserRequestDataDownloadJob::requireHttpAuthentication() const
{
    return true;
}

#include "moc_userrequestdatadownloadjob.cpp"
