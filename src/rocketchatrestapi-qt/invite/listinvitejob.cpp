/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listinvitejob.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ListInviteJob::ListInviteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ListInviteJob::~ListInviteJob() = default;

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
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("ListInviteJob: Ask for displaying all invite link url"));
    return true;
}

void ListInviteJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    addLoggerInfo(QByteArrayLiteral("ListInviteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
    Q_EMIT listInviteDone(replyJson);
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

#include "moc_listinvitejob.cpp"
