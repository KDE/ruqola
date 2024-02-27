/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "methodcalljob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
MethodCallJob::MethodCallJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

MethodCallJob::~MethodCallJob() = default;

bool MethodCallJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("MethodCallJob::start");
    submitPostRequest(json());

    return true;
}

void MethodCallJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("MethodCallJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT methodCallDone(replyObject[QLatin1String("application")].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("MethodCallJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

MethodCallJob::MethodCallJobInfo MethodCallJob::methodCallJobInfo() const
{
    return mMethodCallJobInfo;
}

void MethodCallJob::setMethodCallJobInfo(const MethodCallJobInfo &newMethodCallJobInfo)
{
    mMethodCallJobInfo = newMethodCallJobInfo;
}

bool MethodCallJob::requireHttpAuthentication() const
{
    return !mMethodCallJobInfo.anonymous;
}

bool MethodCallJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mMethodCallJobInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MethodCallJob: mMethodCallJobInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest MethodCallJob::request() const
{
    const QUrl url =
        mRestApiMethod->generateUrl(mMethodCallJobInfo.anonymous ? RestApiUtil::RestApiUrlType::MethodCallAnon : RestApiUtil::RestApiUrlType::MethodCall,
                                    mMethodCallJobInfo.methodName);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument MethodCallJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("message")] = mMethodCallJobInfo.messageObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool MethodCallJob::MethodCallJobInfo::isValid() const
{
    // TODO verify if messageObj is empty
    return !methodName.isEmpty() && !messageObj.isEmpty();
}

#include "moc_methodcalljob.cpp"
