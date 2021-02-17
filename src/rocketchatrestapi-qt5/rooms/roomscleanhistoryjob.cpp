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

#include "roomscleanhistoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoomsCleanHistoryJob::RoomsCleanHistoryJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsCleanHistoryJob::~RoomsCleanHistoryJob()
{
}

bool RoomsCleanHistoryJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelCleanHistoryJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RoomsCleanHistoryJob::slotCleanHistoryFinished);
    return true;
}

void RoomsCleanHistoryJob::slotCleanHistoryFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelCleanHistoryJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT cleanHistoryDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelCleanHistoryJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

RoomsCleanHistoryJob::CleanHistoryInfo RoomsCleanHistoryJob::cleanHistoryInfo() const
{
    return mCleanHistoryInfo;
}

void RoomsCleanHistoryJob::setCleanHistoryInfo(const CleanHistoryInfo &cleanHistoryInfo)
{
    mCleanHistoryInfo = cleanHistoryInfo;
}

bool RoomsCleanHistoryJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsCleanHistoryJob::canStart() const
{
    if (!mCleanHistoryInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mCleanHistoryInfo: mCleanHistoryInfo is not valid.";
        return false;
    }

    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument RoomsCleanHistoryJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mCleanHistoryInfo.roomId;
    if (mCleanHistoryInfo.inclusive) {
        jsonObj[QLatin1String("inclusive")] = true;
    }
    if (mCleanHistoryInfo.ignoreThreads) {
        jsonObj[QLatin1String("ignoreThreads")] = true;
    }
    if (mCleanHistoryInfo.filesOnly) {
        jsonObj[QLatin1String("filesOnly")] = true;
    }
    if (mCleanHistoryInfo.excludePinned) {
        jsonObj[QLatin1String("excludePinned")] = true;
    }
    if (mCleanHistoryInfo.ignoreDiscussion) {
        jsonObj[QLatin1String("ignoreDiscussion")] = true;
    }

    jsonObj[QLatin1String("latest")] = mCleanHistoryInfo.latest.toString(Qt::ISODateWithMs);
    jsonObj[QLatin1String("oldest")] = mCleanHistoryInfo.oldest.toString(Qt::ISODateWithMs);
    if (!mCleanHistoryInfo.users.isEmpty()) {
        jsonObj[QLatin1String("users")] = QJsonArray::fromStringList(mCleanHistoryInfo.users);
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

QNetworkRequest RoomsCleanHistoryJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsCleanHistory);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool RoomsCleanHistoryJob::CleanHistoryInfo::isValid() const
{
    return latest.isValid() && oldest.isValid() && !roomId.isEmpty();
}

QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &t)
{
    d << "latest " << t.latest;
    d << "oldest " << t.oldest;
    d << "roomId " << t.roomId;
    d << "users " << t.users;
    d << "inclusive " << t.inclusive;
    d << "excludePinned " << t.excludePinned;
    d << "filesOnly " << t.filesOnly;
    d << "ignoreThreads " << t.ignoreThreads;
    return d;
}
