/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licenseslistjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
LicensesListJob::LicensesListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LicensesListJob::~LicensesListJob() = default;

bool LicensesListJob::requireHttpAuthentication() const
{
    return true;
}

bool LicensesListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LicensesListJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("LicensesListJob: Ask for licenses list"_ba);
    return true;
}

void LicensesListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("LicensesListJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT licensesListDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("LicensesListJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest LicensesListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::LicensesGet);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_licenseslistjob.cpp"
