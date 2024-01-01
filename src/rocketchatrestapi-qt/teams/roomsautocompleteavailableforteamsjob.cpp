/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("RoomsAutocompleteAvailableForTeamsJob: Ask all rooms"));
    return true;
}

void RoomsAutocompleteAvailableForTeamsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomsAutocompleteAvailableForTeamsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsAutoCompleteChannelAndPrivateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomsAutocompleteAvailableForTeamsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_roomsautocompleteavailableforteamsjob.cpp"
