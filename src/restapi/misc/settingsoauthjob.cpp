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

#include "settingsoauthjob.h"
#include "restapimethod.h"
#include "restapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

SettingsOauthJob::SettingsOauthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SettingsOauthJob::~SettingsOauthJob()
{
}

bool SettingsOauthJob::requireHttpAuthentication() const
{
    return false;
}

bool SettingsOauthJob::start()
{
    if (!canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start SettingsOauthJob";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &SettingsOauthJob::slotSettingsOauthFinished);
    addLoggerInfo(QByteArrayLiteral("SettingsOauthJob: Ask settings oauth"));
    return true;
}

void SettingsOauthJob::slotSettingsOauthFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SettingsOauthJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT settingsOauthDone(replyObject);
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to get oauth settings" << data;
        }
    }
    deleteLater();
}

QNetworkRequest SettingsOauthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsOauth);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}
