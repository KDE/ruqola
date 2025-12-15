/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesisenterprisejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
LicensesIsEnterpriseJob::LicensesIsEnterpriseJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LicensesIsEnterpriseJob::~LicensesIsEnterpriseJob() = default;

bool LicensesIsEnterpriseJob::requireHttpAuthentication() const
{
    return true;
}

bool LicensesIsEnterpriseJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LicensesIsEnterpriseJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("LicensesIsEnterpriseJob: Ask for licenses"_ba);
    return true;
}

void LicensesIsEnterpriseJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("LicensesIsEnterpriseJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT licensesIsEnterpriseDone(replyObject["isEnterprise"_L1].toBool());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("LicensesIsEnterpriseJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest LicensesIsEnterpriseJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::LicensesIsEntreprise);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_licensesisenterprisejob.cpp"
