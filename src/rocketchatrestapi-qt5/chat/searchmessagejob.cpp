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

#include "searchmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
SearchMessageJob::SearchMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SearchMessageJob::~SearchMessageJob()
{
}

bool SearchMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool SearchMessageJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start search message job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &SearchMessageJob::slotSearchMessageFinished);
    addLoggerInfo(QByteArrayLiteral("SearchMessageJob: search message starting"));
    return true;
}

void SearchMessageJob::slotSearchMessageFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SearchMessageJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT searchMessageDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SearchMessageJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

int SearchMessageJob::count() const
{
    return mCount;
}

void SearchMessageJob::setCount(int count)
{
    mCount = count;
}

QString SearchMessageJob::searchText() const
{
    return mSearchText;
}

void SearchMessageJob::setSearchText(const QString &searchText)
{
    mSearchText = searchText;
}

QString SearchMessageJob::roomId() const
{
    return mRoomId;
}

void SearchMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest SearchMessageJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatSearch);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    queryUrl.addQueryItem(QStringLiteral("searchText"), mSearchText);
    if (mCount != -1) {
        queryUrl.addQueryItem(QStringLiteral("count"), QString::number(mCount));
    }
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
#else
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
#endif
    return request;
}

bool SearchMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is empty";
        return false;
    }
    if (mSearchText.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SearchText is empty";
        return false;
    }
    return true;
}
