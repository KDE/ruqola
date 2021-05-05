/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "listinvitejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ListInviteJob::ListInviteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ListInviteJob::~ListInviteJob()
{
}

bool ListInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool ListInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();

    connect(reply, &QNetworkReply::finished, this, &ListInviteJob::slotListInviteFinished);
    addStartRestApiInfo(QByteArrayLiteral("ListInviteJob: Ask for displaying all invite link url"));
    return true;
}

void ListInviteJob::slotListInviteFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        addLoggerInfo(QByteArrayLiteral("ListInviteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT listInviteDone(replyJson);
        reply->deleteLater();
    }
    deleteLater();
}

bool ListInviteJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest ListInviteJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ListInvites);
    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool ListInviteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}
