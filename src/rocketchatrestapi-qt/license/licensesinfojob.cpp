/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
LicensesInfoJob::LicensesInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LicensesInfoJob::~LicensesInfoJob() = default;

bool LicensesInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool LicensesInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LicensesInfoJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("LicensesInfoJob: Ask for licenses info."_ba);
    return true;
}

void LicensesInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("LicensesInfoJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT licensesInfoDone(*replyObject);
    }
}

QNetworkRequest LicensesInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::LicensesInfo);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_licensesinfojob.cpp"
