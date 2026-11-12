/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "spotlightjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
SpotlightJob::SpotlightJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SpotlightJob::~SpotlightJob() = default;

bool SpotlightJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SpotlightJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("SpotlightJob start"_ba);
    return true;
}

void SpotlightJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("SpotlightJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT spotlightDone(*replyObject);
    }
}

SpotlightJob::SpotlightJobInfo SpotlightJob::spotlightInfo() const
{
    return mSpotlightInfo;
}

void SpotlightJob::setSpotlightInfo(const SpotlightJobInfo &newSpotlightInfo)
{
    mSpotlightInfo = newSpotlightInfo;
}

bool SpotlightJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest SpotlightJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Spotlight);
    QUrlQuery queryUrl;
    mSpotlightInfo.addQueryItem(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool SpotlightJob::canStart() const
{
    if (!mSpotlightInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SpotlightJob: spotlight info is invalid";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool SpotlightJob::SpotlightJobInfo::isValid() const
{
    if (QStringView(pattern).trimmed().isEmpty()) {
        return false;
    }
    return true;
}

QString SpotlightJob::SpotlightJobInfo::convertSpotlightTypeToString() const
{
    QStringList lst;
    if (types & SpotlightJobType::Users) {
        lst << u"\"users\":true"_s;
    }
    if (types & SpotlightJobType::Rooms) {
        lst << u"\"rooms\":true"_s;
    }
    if (types & SpotlightJobType::Mentions) {
        lst << u"\"mentions\":true"_s;
    }
    return u'{' + lst.join(u',') + u'}';
}

void SpotlightJob::SpotlightJobInfo::addQueryItem(QUrlQuery &queryUrl) const
{
    queryUrl.addQueryItem(u"query"_s, pattern);
    if (!roomId.isEmpty()) {
        queryUrl.addQueryItem(u"rid"_s, QString::fromLatin1(roomId));
    }
    if (types != SpotlightJobType::Unknown) {
        queryUrl.addQueryItem(u"type"_s, convertSpotlightTypeToString());
    }
    if (!excludeUsernames.isEmpty()) {
        queryUrl.addQueryItem(u"usernames"_s, excludeUsernames.join(u','));
    }
}

QDebug operator<<(QDebug d, const RocketChatRestApi::SpotlightJob::SpotlightJobInfo &t)
{
    d.space() << "pattern:" << t.pattern;
    d.space() << "excludeUsernames:" << t.excludeUsernames;
    d.space() << "types:" << t.types;
    d.space() << "roomId:" << t.roomId;
    return d;
}

#include "moc_spotlightjob.cpp"
