/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "registeruserjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
RegisterUserJob::RegisterUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RegisterUserJob::~RegisterUserJob()
{
}

bool RegisterUserJob::canStart() const
{
    if (!mRegisterUserInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mRegisterUserInfo is empty"; //TODO improve it.
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool RegisterUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RegisterUserJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RegisterUserJob::slotForgotPassword);
    return true;
}

void RegisterUserJob::slotForgotPassword()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RegisterUserJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT forgotPasswordDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RegisterUserJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

RegisterUserJob::RegisterUserInfo RegisterUserJob::registerUserInfo() const
{
    return mRegisterUserInfo;
}

void RegisterUserJob::setRegisterUserInfo(const RegisterUserInfo &registerUserInfo)
{
    mRegisterUserInfo = registerUserInfo;
}

QNetworkRequest RegisterUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersRegister);
    QNetworkRequest request(url);
    addRequestAttribute(request);
    return request;
}

bool RegisterUserJob::requireHttpAuthentication() const
{
    return false;
}

QJsonDocument RegisterUserJob::json() const
{
    QJsonObject jsonObj;
    //jsonObj[QLatin1String("email")] = mEmail;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool RegisterUserJob::RegisterUserInfo::isValid() const
{
    return !username.trimmed().isEmpty() &&
            !name.trimmed().isEmpty() &&
            !email.trimmed().isEmpty() &&
            !password.trimmed().isEmpty();
}
