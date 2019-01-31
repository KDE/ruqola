/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "privateinfojob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
PrivateInfoJob::PrivateInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PrivateInfoJob::~PrivateInfoJob()
{
}

bool PrivateInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PrivateInfoJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo(QByteArrayLiteral("PrivateInfoJob: Ask private info"));
    connect(reply, &QNetworkReply::finished, this, &PrivateInfoJob::slotPrivateInfoDone);

    return true;
}

void PrivateInfoJob::slotPrivateInfoDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("PrivateInfoJob done: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT privateInfoDone(data);
        } else {
            addLoggerWarning(QByteArrayLiteral("PrivateInfoJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

bool PrivateInfoJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest PrivateInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Settings);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}
