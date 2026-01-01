/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionslistalljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
PermissionsListAllJob::PermissionsListAllJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PermissionsListAllJob::~PermissionsListAllJob() = default;

bool PermissionsListAllJob::requireHttpAuthentication() const
{
    return true;
}

bool PermissionsListAllJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PermissionsListAllJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("ListPermissionsJob: Ask info about me"_ba);
    return true;
}

void PermissionsListAllJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ListPermissionsJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT permissionListAllDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ListPermissionsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QDateTime &PermissionsListAllJob::updatedSince() const
{
    return mUpdatedSince;
}

void PermissionsListAllJob::setUpdatedSince(const QDateTime &newUpdatedSince)
{
    mUpdatedSince = newUpdatedSince;
}

QNetworkRequest PermissionsListAllJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::PermissionsListAll);
    if (mUpdatedSince.isValid()) {
        // TODO use mUpdatedSince (Date as ISO string)
        // permissions.listAll?updatedSince=2017-11-25T15:08:17.248Z
    }
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_permissionslistalljob.cpp"
