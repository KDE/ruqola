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

#include "owninfojob.h"
#include "ruqola_ddpapi_debug.h"
#include "restapimethod.h"
#include "restapirequest.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

OwnInfoJob::OwnInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{

}

OwnInfoJob::~OwnInfoJob()
{

}

bool OwnInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool OwnInfoJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Impossible to start owninfo job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &OwnInfoJob::slotServerInfoFinished);
    reply->setProperty("method", QVariant::fromValue(RestApiRequest::RestMethod::Me));
    return true;
}

void OwnInfoJob::slotServerInfoFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        Q_EMIT ownInfoDone(data);
    }
    deleteLater();
}

QNetworkRequest OwnInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServerInfo);
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}
