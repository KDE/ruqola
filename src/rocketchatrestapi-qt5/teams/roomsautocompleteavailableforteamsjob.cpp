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

#include "roomsautocompleteavailableforteamsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
RoomsAutocompleteAvailableForTeamsJob::RoomsAutocompleteAvailableForTeamsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsAutocompleteAvailableForTeamsJob::~RoomsAutocompleteAvailableForTeamsJob() = default;

bool RoomsAutocompleteAvailableForTeamsJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsAutocompleteAvailableForTeamsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsAutocompleteAvailableForTeamsJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &RoomsAutocompleteAvailableForTeamsJob::slotRoomsAutoCompleteChannelAndPrivateFinished);
    addStartRestApiInfo(QByteArrayLiteral("RoomsAutocompleteAvailableForTeamsJob: Ask all rooms"));
    return true;
}

void RoomsAutocompleteAvailableForTeamsJob::slotRoomsAutoCompleteChannelAndPrivateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoomsAutocompleteAvailableForTeamsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT roomsAutoCompleteChannelAndPrivateDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoomsAutocompleteAvailableForTeamsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest RoomsAutocompleteAvailableForTeamsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteAvailableForTeams);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("name"), mRoomsAutocompleteInfo.name);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

RoomsAutocompleteAvailableForTeamsJob::RoomsAutocompleteChannelAndPrivateInfo RoomsAutocompleteAvailableForTeamsJob::roomsCompleterInfo() const
{
    return mRoomsAutocompleteInfo;
}

void RoomsAutocompleteAvailableForTeamsJob::setRoomsCompleterInfo(
    const RoomsAutocompleteAvailableForTeamsJob::RoomsAutocompleteChannelAndPrivateInfo &roomCompleterInfo)
{
    mRoomsAutocompleteInfo = roomCompleterInfo;
}

bool RoomsAutocompleteAvailableForTeamsJob::RoomsAutocompleteChannelAndPrivateInfo::isValid() const
{
    return !name.isEmpty();
}
