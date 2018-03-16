/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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
#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include "restapirequest.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

ServerInfoJob::ServerInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{

}

ServerInfoJob::~ServerInfoJob()
{

}

bool ServerInfoJob::start()
{
    if (!mNetworkAccessManager) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Network manager not defined";
        return false;
    }
    if (!mRestApiMethod) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "RestaApiMethod not defined";
        return false;
    }
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServerInfo);
    qDebug() << " url " << url;
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    QNetworkReply *reply = mNetworkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ServerInfoJob::slotServerInfoFinished);
    //TODO remove it after porting
    reply->setProperty("method", QVariant::fromValue(RestApiRequest::RestMethod::ServerInfo));

    return true;
}

void ServerInfoJob::slotServerInfoFinished()
{
    QString versionStr;
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        qCDebug(RUQOLA_RESTAPI_LOG) << "ServerInfoJob::parseServerInfo: " << data;
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        const QJsonObject version = replyObject.value(QStringLiteral("info")).toObject();
        versionStr = version.value(QStringLiteral("version")).toString();
        if (versionStr.isEmpty()) {
            qCWarning(RUQOLA_RESTAPI_LOG) << "ServerInfoJob::slotServerInfoFinished Problem during parsing server version";
        }
    }
    Q_EMIT serverInfoDone(versionStr);
    deleteLater();
}
