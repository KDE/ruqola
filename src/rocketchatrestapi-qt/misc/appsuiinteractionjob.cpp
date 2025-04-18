/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsuiinteractionjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
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

QDebug operator<<(QDebug d, const RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo &t)
{
    d << " messageObj: " << t.messageObj;
    d << " methodName: " << t.methodName;
    return d;
}

void AppsUiInteractionJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    // qDebug() << " response " << replyErrorString << "replyJson  " << replyJson;
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("AppsUiInteractionJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appsUiInteractionDone(replyObject);
    } else {
        Q_EMIT appsUiInteractionFailed(replyObject);
        // emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AppsUiInteractionJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::AppsUiInteraction,
                                                 RestApiUtil::RestApiUrlExtensionType::Apps,
                                                 mAppsUiInteractionJobInfo.methodName);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument AppsUiInteractionJob::json() const
{
    const QJsonObject jsonObj = mAppsUiInteractionJobInfo.messageObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool AppsUiInteractionJob::AppsUiInteractionJobInfo::isValid() const
{
    // TODO verify if messageObj is empty
    return !methodName.isEmpty() && !messageObj.isEmpty();
}

void AppsUiInteractionJob::AppsUiInteractionJobInfo::generateMessageObj(const QString &actionId,
                                                                        const QString &value,
                                                                        const QString &blockId,
                                                                        const QByteArray &roomId,
                                                                        const QByteArray &messageId)
{
    QJsonObject o;
    o.insert(QStringLiteral("type"), QStringLiteral("blockAction"));
    o.insert(QStringLiteral("actionId"), actionId);

    QJsonObject payload;
    payload.insert(QStringLiteral("blockId"), blockId);
    payload.insert(QStringLiteral("value"), value);

    o.insert(QStringLiteral("payload"), payload);
    o.insert(QStringLiteral("rid"), QLatin1StringView(roomId));
    o.insert(QStringLiteral("mid"), QLatin1StringView(messageId));

    QJsonObject container;
    container.insert(QStringLiteral("type"), QStringLiteral("message"));
    container.insert(QStringLiteral("id"), QLatin1StringView(messageId));
    o.insert(QStringLiteral("container"), container);

    // TODO fix me.
    o.insert(QStringLiteral("triggerId"), QStringLiteral("foo"));
    messageObj = o;
}

#include "moc_appsuiinteractionjob.cpp"
