/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userrequestdatadownloadjob.h"

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UserRequestDataDownloadJob::UserRequestDataDownloadJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UserRequestDataDownloadJob::~UserRequestDataDownloadJob() = default;

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
    if (replyObject["success"_L1].toBool()) {
        // qDebug() << " replyObject " << replyObject;
        addLoggerInfo("UserRequestDataDownloadJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        const QString result = replyObject["result"_L1].toString();
        Q_EMIT userRequestDataDownloadDone(result);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UserRequestDataDownloadJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(u"fullExport"_s, mFullExport ? u"true"_s : u"false"_s);
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
