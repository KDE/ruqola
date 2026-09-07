/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmaxactiveusersjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
LicensesMaxActiveUsersJob::LicensesMaxActiveUsersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LicensesMaxActiveUsersJob::~LicensesMaxActiveUsersJob() = default;

bool LicensesMaxActiveUsersJob::requireHttpAuthentication() const
{
    return true;
}

bool LicensesMaxActiveUsersJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LicensesMaxActiveUsers";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("LicensesMaxActiveUsers: Ask for licenses max actives users."_ba);
    return true;
}

void LicensesMaxActiveUsersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("LicensesMaxActiveUsersJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT licensesMaxActiveUsersDone(*replyObject);
    }
}

QNetworkRequest LicensesMaxActiveUsersJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::LicensesMaxActiveUsers);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_licensesmaxactiveusersjob.cpp"
