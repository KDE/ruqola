/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeinvitejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QNetworkReply>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RemoveInviteJob::RemoveInviteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RemoveInviteJob::~RemoveInviteJob() = default;

bool RemoveInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool RemoveInviteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mIdentifier.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mIdentifier is Empty. Impossible to start RemoveInviteJob";
        return false;
    }
    return true;
}

bool RemoveInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }

    mReply = networkAccessManager()->deleteResource(request());
    const QByteArray className = metaObject()->className();
    mReply->setProperty("jobClassName", className);

    connect(mReply, &QNetworkReply::finished, this, &RemoveInviteJob::slotRemoveInviteFinished);
    addStartRestApiInfo("RemoveInviteJob: Ask for displaying all invite link url"_ba);
    return true;
}

void RemoveInviteJob::slotRemoveInviteFinished()
{
    auto reply = mReply;
    if (reply) {
        if (reply->readAll() == "true") {
            addLoggerInfo("RemoveInviteJob: success: "_ba);
            Q_EMIT removeInviteDone(mIdentifier);
        } else {
            // emitFailedMessage(replyObject, reply);
            addLoggerWarning("RemoveInviteJob: Problem: "_ba);
        }
#if 0
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject["success"_L1].toBool()) {
            addLoggerInfo("RemoveInviteJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeInviteDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning("RemoveInviteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
        }
#endif
        reply->deleteLater();
    }
    deleteLater();
}

QByteArray RemoveInviteJob::identifier() const
{
    return mIdentifier;
}

void RemoveInviteJob::setIdentifier(const QByteArray &identifier)
{
    mIdentifier = identifier;
}

bool RemoveInviteJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest RemoveInviteJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RemoveInvite, RestApiUtil::RestApiUrlExtensionType::V1, QLatin1StringView(mIdentifier));
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_removeinvitejob.cpp"
