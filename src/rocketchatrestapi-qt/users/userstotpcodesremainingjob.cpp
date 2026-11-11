/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userstotpcodesremainingjob.h"

#include "restapimethod.h"

#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersTotpCodesRemainingJob::UsersTotpCodesRemainingJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersTotpCodesRemainingJob::~UsersTotpCodesRemainingJob() = default;

bool UsersTotpCodesRemainingJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersTotpCodesRemainingJob::start"_ba);
    submitGetRequest();
    return true;
}

void UsersTotpCodesRemainingJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("UsersTotpCodesRemainingJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT usersTotpCodesRemainingDone(*replyObject);
    }
}

bool UsersTotpCodesRemainingJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersTotpCodesRemainingJob::hasQueryParameterSupport() const
{
    return false;
}

QNetworkRequest UsersTotpCodesRemainingJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersTotpCodesRemaining);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_userstotpcodesremainingjob.cpp"
