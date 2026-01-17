/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SearchMessageJob::SearchMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SearchMessageJob::~SearchMessageJob() = default;

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
    submitGetRequest();

    addStartRestApiInfo("SearchMessageJob: search message starting"_ba);
    return true;
}

void SearchMessageJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SearchMessageJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT searchMessageDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SearchMessageJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool SearchMessageJob::useRegularExpression() const
{
    return mUseRegularExpression;
}

void SearchMessageJob::setUseRegularExpression(bool useRegularExpression)
{
    mUseRegularExpression = useRegularExpression;
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

QString SearchMessageJob::convertSearchText() const
{
    if (mUseRegularExpression) {
        return u"/%1/i"_s.arg(mSearchText);
    }
    return mSearchText;
}

int SearchMessageJob::offset() const
{
    return mOffset;
}

void SearchMessageJob::setOffset(int newOffset)
{
    mOffset = newOffset;
}

QNetworkRequest SearchMessageJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatSearch);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, mRoomId);
    queryUrl.addQueryItem(u"searchText"_s, convertSearchText());
    if (mCount != -1) {
        queryUrl.addQueryItem(u"count"_s, QString::number(mCount));
    }
    if (mOffset != -1) {
        queryUrl.addQueryItem(u"offset"_s, QString::number(mOffset));
    }
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

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

QDebug operator<<(QDebug d, const RocketChatRestApi::SearchMessageJob &t)
{
    d.space() << "roomId:" << t.roomId();
    d.space() << "searchText:" << t.searchText();
    d.space() << "count:" << t.count();
    d.space() << "useRegularExpression:" << t.useRegularExpression();
    d.space() << "offset:" << t.offset();
    return d;
}
#include "moc_searchmessagejob.cpp"
