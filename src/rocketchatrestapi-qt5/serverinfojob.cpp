/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "serverinfojob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ServerInfoJob::ServerInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ServerInfoJob::~ServerInfoJob()
{
}

bool ServerInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo("ServerInfoJob::start");
    connect(reply, &QNetworkReply::finished, this, &ServerInfoJob::slotServerInfoFinished);

    return true;
}

//Since 2.0.0 we don't use v1 path. Need to exclude it.
QNetworkRequest ServerInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServerInfo, QString(), mUseDeprecatedVersion);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

bool ServerInfoJob::requireHttpAuthentication() const
{
    return false;
}

bool ServerInfoJob::useDeprecatedVersion() const
{
    return mUseDeprecatedVersion;
}

void ServerInfoJob::slotServerInfoFinished()
{
    QString versionStr;
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            if (mUseDeprecatedVersion) {
                const QJsonObject version = replyObject.value(QStringLiteral("info")).toObject();
                versionStr = version.value(QStringLiteral("version")).toString();
                addLoggerInfo(QByteArrayLiteral("ServerInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
                Q_EMIT serverInfoDone(versionStr);
            } else {
                versionStr = replyObject.value(QStringLiteral("version")).toString();
                addLoggerInfo(QByteArrayLiteral("ServerInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
                Q_EMIT serverInfoDone(versionStr);
            }
        } else {
            Q_EMIT serverInfoFailed(mUseDeprecatedVersion);
            addLoggerWarning(QByteArrayLiteral("ServerInfoJob::slotServerInfoFinished: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
            if (!mUseDeprecatedVersion) {
                emitFailedMessage(replyObject, reply);
            }
        }
        reply->deleteLater();
    }
    deleteLater();
}

void ServerInfoJob::setUseDeprecatedVersion(bool useDeprecatedVersion)
{
    mUseDeprecatedVersion = useDeprecatedVersion;
}
