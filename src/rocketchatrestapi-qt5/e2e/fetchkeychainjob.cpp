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

#include "fetchkeychainjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
FetchKeyChainJob::FetchKeyChainJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FetchKeyChainJob::~FetchKeyChainJob()
{
}

bool FetchKeyChainJob::canStart() const
{
//    if (mPresenceUserId.isEmpty()) {
//        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "presenceuserid is empty";
//        return false;
//    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start FetchKeyChain job";
        return false;
    }
    return true;
}

bool FetchKeyChainJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &FetchKeyChainJob::slotFetchKeyChain);
    addLoggerInfo("FetchKeyChainJob ask for presenceUserId: "/* + mPresenceUserId.toLatin1()*/);

    return true;
}

void FetchKeyChainJob::slotFetchKeyChain()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("FetchKeyChainJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
            //TODO
            Q_EMIT fetchKeyChainDone();
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) <<" Problem when we tried to get username suggestion" << data;
        }
    }
    deleteLater();
}

QNetworkRequest FetchKeyChainJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EfetchKeychain);
//    QUrlQuery queryUrl;
//    queryUrl.addQueryItem(QStringLiteral("userId"), mPresenceUserId);
//    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    return request;
}

bool FetchKeyChainJob::requireHttpAuthentication() const
{
    return true;
}

