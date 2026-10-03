/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsautocompleteavailableforteamsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
    submitGetRequest();

    addStartRestApiInfo("RoomsAutocompleteAvailableForTeamsJob: Ask all rooms"_ba);
    return true;
}

void RoomsAutocompleteAvailableForTeamsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("RoomsAutocompleteAvailableForTeamsJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT roomsAutoCompleteChannelAndPrivateDone(*replyObject);
    }
}

QNetworkRequest RoomsAutocompleteAvailableForTeamsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteAvailableForTeams);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"name"_s, mRoomsAutocompleteInfo.name);
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

#include "moc_roomsautocompleteavailableforteamsjob.cpp"
