/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "restapirequest.h"
#include "ruqola_debug.h"
#include "restapiutil.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

RestApiRequest::RestApiRequest(QObject *parent)
    : QObject(parent)
{
    mNetworkAccessManager = new QNetworkAccessManager(this);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &RestApiRequest::slotResult);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &RestApiRequest::slotSslErrors);
}

RestApiRequest::~RestApiRequest()
{

}

void RestApiRequest::parseLogin(const QByteArray &data)
{
    QJsonDocument replyJson = QJsonDocument::fromJson(data);
    QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("status")].toString() == QStringLiteral("success") && replyObject.contains(QStringLiteral("data"))) {
        QJsonObject data = replyObject[QStringLiteral("data")].toObject();

        if ( data.contains(QStringLiteral("authToken")) && data.contains(QStringLiteral("userId"))) {
            mAuthToken = data[QStringLiteral("authToken")].toString();
            mUserId = data[QStringLiteral("userId")].toString();
            if (!mSearchRoom) {
                channelList();
                mSearchRoom = true;
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Error during login" << data;
    }
}

void RestApiRequest::parseLogout(const QByteArray &data)
{
    qDebug() << " void RestApiRequest::parseLogout(const QByteArray &data)" << data;
}

void RestApiRequest::parseChannelList(const QByteArray &data)
{
    qDebug() << " void RestApiRequest::parseChannelList(const QByteArray &data)" << data;
}

void RestApiRequest::slotResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        const QByteArray data = reply->readAll();
        const RestMethod restMethod = reply->property("method").value<RestMethod>();
        switch (restMethod) {
        case Login:
            parseLogin(data);
            break;
        case Logout:
            parseLogout(data);
            break;
        case ChannelList:
            parseChannelList(data);
            break;
        case Unknown:
            qWarning() << " Unknown restapi method" << data;
            break;
        }
    } else {
        qDebug() << " reply - "<<reply->errorString();
    }
    reply->deleteLater();
}

void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    qCDebug(RUQOLA_LOG) << " void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)" << error.count();
    reply->ignoreSslErrors(error);
}

void RestApiRequest::setPassword(const QString &password)
{
    mPassword = password;
}

void RestApiRequest::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString RestApiRequest::serverUrl() const
{
    return mServerUrl;
}

void RestApiRequest::setServerUrl(const QString &serverUrl)
{
    mServerUrl = serverUrl;
}

QString RestApiRequest::authToken() const
{
    return mAuthToken;
}

QString RestApiRequest::userId() const
{
    return mUserId;
}

void RestApiRequest::login()
{
    if (!mUserName.isEmpty() && !mPassword.isEmpty() && !mServerUrl.isEmpty()) {
        QUrl url = QUrl(RestApiUtil::adaptUrl(mServerUrl) + QStringLiteral("/api/v1/login"));
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

        QVariantMap loginMap;
        loginMap.insert(QStringLiteral("user"), mUserName);
        loginMap.insert(QStringLiteral("password"), mPassword);
        const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
        const QByteArray baPostData = postData.toJson(QJsonDocument::Compact);
        QNetworkReply *reply = mNetworkAccessManager->post(request, baPostData);
        reply->setProperty("method", QVariant::fromValue(RestMethod::Login));
        //connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &CreatePhishingUrlDataBaseJob::slotError);
    } else {
        qCWarning(RUQOLA_LOG) << "Password or user or url is empty";
    }
}

void RestApiRequest::logout()
{
    const QUrl url = QUrl(RestApiUtil::adaptUrl(mServerUrl) + QStringLiteral("/api/v1/logout"));
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    QNetworkReply *reply = mNetworkAccessManager->get(request);
    reply->setProperty("method", QVariant::fromValue(RestMethod::Logout));
}

void RestApiRequest::channelList()
{
    const QUrl url = QUrl(RestApiUtil::adaptUrl(mServerUrl) + QStringLiteral("/api/v1/channels.list"));
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    QNetworkReply *reply = mNetworkAccessManager->get(request);
    reply->setProperty("method", QVariant::fromValue(RestMethod::ChannelList));
}
