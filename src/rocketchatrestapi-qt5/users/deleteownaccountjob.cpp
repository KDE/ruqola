/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "deleteownaccountjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
DeleteOwnAccountJob::DeleteOwnAccountJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DeleteOwnAccountJob::~DeleteOwnAccountJob() = default;

bool DeleteOwnAccountJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteOwnAccountJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &DeleteOwnAccountJob::slotDeleteOwnAccount);
    return true;
}

void DeleteOwnAccountJob::slotDeleteOwnAccount()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("DeleteOwnAccountJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT deleteOwnAccountDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("DeleteOwnAccountJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString DeleteOwnAccountJob::password() const
{
    return mPassword;
}

void DeleteOwnAccountJob::setPassword(const QString &password)
{
    mPassword = password;
}

bool DeleteOwnAccountJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteOwnAccountJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mPassword.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteOwnAccountJob: mPassword is empty";
        return false;
    }
    return true;
}

QNetworkRequest DeleteOwnAccountJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersDeleteOwnAccount);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument DeleteOwnAccountJob::json() const
{
    QJsonObject jsonObj;
    const QByteArray sha256pw = RestApiUtil::convertSha256Password(mPassword);
    jsonObj[QLatin1String("password")] = QString::fromLatin1(sha256pw);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
