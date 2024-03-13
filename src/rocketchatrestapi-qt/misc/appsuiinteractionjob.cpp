/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsuiinteractionjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
AppsUiInteractionJob::AppsUiInteractionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AppsUiInteractionJob::~AppsUiInteractionJob() = default;

bool AppsUiInteractionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("AppsUiInteractionJob::start");
    submitPostRequest(json());
    return true;
}

void AppsUiInteractionJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    // qDebug() << " response " << replyErrorString << "replyJson  " << replyJson;
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppsUiInteractionJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        const QJsonObject obj = QJsonDocument::fromJson(replyObject[QLatin1String("message")].toString().toUtf8()).object();
        Q_EMIT AppsUiInteractionDone(obj);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppsUiInteractionJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

AppsUiInteractionJob::AppsUiInteractionJobInfo AppsUiInteractionJob::methodCallJobInfo() const
{
    return mAppsUiInteractionJobInfo;
}

void AppsUiInteractionJob::setAppsUiInteractionJobInfo(const AppsUiInteractionJobInfo &newAppsUiInteractionJobInfo)
{
    mAppsUiInteractionJobInfo = newAppsUiInteractionJobInfo;
}

bool AppsUiInteractionJob::requireHttpAuthentication() const
{
    return true;
}

bool AppsUiInteractionJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mAppsUiInteractionJobInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AppsUiInteractionJob: mAppsUiInteractionJobInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest AppsUiInteractionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::AppsUiInteraction, mAppsUiInteractionJobInfo.methodName, false);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument AppsUiInteractionJob::json() const
{
    QJsonObject jsonObj;
    // We need to convert to string
    jsonObj[QLatin1String("message")] = QString::fromUtf8(QJsonDocument(mAppsUiInteractionJobInfo.messageObj).toJson(QJsonDocument::Compact));
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool AppsUiInteractionJob::AppsUiInteractionJobInfo::isValid() const
{
    // TODO verify if messageObj is empty
    return !methodName.isEmpty() && !messageObj.isEmpty();
}

#include "moc_appsuiinteractionjob.cpp"
