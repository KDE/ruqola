/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ServerInfoJob::ServerInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ServerInfoJob::~ServerInfoJob() = default;

bool ServerInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    submitGetRequest();
    addStartRestApiInfo("ServerInfoJob::start");
    return true;
}

// Since 2.0.0 we don't use v1 path. Need to exclude it.
QNetworkRequest ServerInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServerInfo, QString(), mUseDeprecatedVersion);
    QNetworkRequest request(url);
    if (mForceRequiresAuthentication) {
        addAuthRawHeader(request);
    }
    addRequestAttribute(request, false);

    return request;
}

bool ServerInfoJob::requireHttpAuthentication() const
{
    return mForceRequiresAuthentication;
}

bool ServerInfoJob::useDeprecatedVersion() const
{
    return mUseDeprecatedVersion;
}

void ServerInfoJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    // TODO send replyObject too. Need by administrator server info.
    if (replyObject[QStringLiteral("success")].toBool()) {
        QString versionStr;
        if (mUseDeprecatedVersion) {
            const QJsonObject version = replyObject.value(QStringLiteral("info")).toObject();
            versionStr = version.value(QStringLiteral("version")).toString();
            addLoggerInfo(QByteArrayLiteral("ServerInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT serverInfoDone(versionStr, replyObject);
        } else {
            versionStr = replyObject.value(QStringLiteral("version")).toString();
            addLoggerInfo(QByteArrayLiteral("ServerInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT serverInfoDone(versionStr, replyObject);
        }
    } else {
        Q_EMIT serverInfoFailed(mUseDeprecatedVersion);
        addLoggerWarning(QByteArrayLiteral("ServerInfoJob::slotServerInfoFinished: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        if (!mUseDeprecatedVersion) {
            emitFailedMessage(replyObject);
        }
    }
}

bool ServerInfoJob::forceRequiresAuthentication() const
{
    return mForceRequiresAuthentication;
}

void ServerInfoJob::setForceRequiresAuthentication(bool forceRequiresAuthentication)
{
    mForceRequiresAuthentication = forceRequiresAuthentication;
}

void ServerInfoJob::setUseDeprecatedVersion(bool useDeprecatedVersion)
{
    mUseDeprecatedVersion = useDeprecatedVersion;
}
