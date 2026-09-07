/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getroomsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
GetRoomsJob::GetRoomsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetRoomsJob::~GetRoomsJob() = default;

bool GetRoomsJob::requireHttpAuthentication() const
{
    return true;
}

bool GetRoomsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getrooms job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("GetRoomsJob: Ask all rooms"_ba);
    return true;
}

void GetRoomsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("GetRoomsJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT getRoomsDone(*replyObject);
    }
}

QNetworkRequest GetRoomsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsGet);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_getroomsjob.cpp"
