/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsuiinteractionjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
using namespace Qt::Literals::StringLiterals;
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
    addStartRestApiInfo("AppsUiInteractionJob::start"_ba);
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
    o.insert("type"_L1, u"blockAction"_s);
    o.insert("actionId"_L1, actionId);

    QJsonObject payload;
    payload.insert("blockId"_L1, blockId);
    payload.insert("value"_L1, value);

    o.insert("payload"_L1, std::move(payload));
    o.insert("rid"_L1, QLatin1StringView(roomId));
    o.insert("mid"_L1, QLatin1StringView(messageId));

    QJsonObject container;
    container.insert("type"_L1, u"message"_s);
    container.insert("id"_L1, QLatin1StringView(messageId));
    o.insert("container"_L1, std::move(container));

    // TODO fix me.
    o.insert("triggerId"_L1, u"foo"_s);
    messageObj = std::move(o);
}

#include "moc_appsuiinteractionjob.cpp"
