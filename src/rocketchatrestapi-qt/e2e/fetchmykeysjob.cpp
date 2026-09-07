/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fetchmykeysjob.h"

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
FetchMyKeysJob::FetchMyKeysJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FetchMyKeysJob::~FetchMyKeysJob() = default;

bool FetchMyKeysJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("Start FetchMyKeysJob"_ba);

    return true;
}

void FetchMyKeysJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("FetchMyKeysJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT fetchMyKeysDone(*replyObject);
    }
}

QNetworkRequest FetchMyKeysJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EfetchMyKeys);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool FetchMyKeysJob::requireHttpAuthentication() const
{
    return true;
}

#include "moc_fetchmykeysjob.cpp"
