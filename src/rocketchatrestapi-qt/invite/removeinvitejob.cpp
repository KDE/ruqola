/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeinvitejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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
    addStartRestApiInfo(QByteArrayLiteral("RemoveInviteJob: Ask for displaying all invite link url"));
    return true;
}

void RemoveInviteJob::slotRemoveInviteFinished()
{
    auto reply = mReply;
    if (reply) {
        if (reply->readAll() == "true") {
            addLoggerInfo(QByteArrayLiteral("RemoveInviteJob: success: "));
            Q_EMIT removeInviteDone(mIdentifier);
        } else {
            // emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RemoveInviteJob: Problem: "));
        }
#if 0
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QLatin1String("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RemoveInviteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeInviteDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RemoveInviteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
#endif
        reply->deleteLater();
    }
    deleteLater();
}

QString RemoveInviteJob::identifier() const
{
    return mIdentifier;
}

void RemoveInviteJob::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

bool RemoveInviteJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest RemoveInviteJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RemoveInvite, mIdentifier);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

#include "moc_removeinvitejob.cpp"
