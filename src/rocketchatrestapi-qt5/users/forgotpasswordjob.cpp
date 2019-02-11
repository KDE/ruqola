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

#include "forgotpasswordjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ForgotPasswordJob::ForgotPasswordJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ForgotPasswordJob::~ForgotPasswordJob()
{
}

bool ForgotPasswordJob::canStart() const
{
    if (mEmail.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Avatar email is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getavatar job";
        return false;
    }
    return true;
}

bool ForgotPasswordJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ForgotPasswordJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ForgotPasswordJob::slotForgotPassword);
    return true;
}

void ForgotPasswordJob::slotForgotPassword()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        //qCDebug(ROCKETCHATQTRESTAPI_LOG) << "RestApiRequest::parseGetAvatar: " << data << " userId "<<userId;
        addLoggerInfo(QByteArrayLiteral("ForgotPasswordJob: success: ") + data);
        Q_EMIT forgotPasswordDone();
    }
    deleteLater();
}

QString ForgotPasswordJob::email() const
{
    return mEmail;
}

void ForgotPasswordJob::setEmail(const QString &email)
{
    mEmail = email;
}

QNetworkRequest ForgotPasswordJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ForgotPassword);
    QNetworkRequest request(url);
    return request;
}

bool ForgotPasswordJob::requireHttpAuthentication() const
{
    return false;
}

QJsonDocument ForgotPasswordJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("email")] = mEmail;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
