/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

RemoveInviteJob::~RemoveInviteJob()
{
}

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
    QNetworkReply *reply = submitDeleteRequest();

    connect(reply, &QNetworkReply::finished, this, &RemoveInviteJob::slotRemoveInviteFinished);
    addStartRestApiInfo(QByteArrayLiteral("RemoveInviteJob: Ask for displaying all invite link url"));
    return true;
}

void RemoveInviteJob::slotRemoveInviteFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        if (reply->readAll() == "true") {
            addLoggerInfo(QByteArrayLiteral("RemoveInviteJob: success: "));
            Q_EMIT removeInviteDone();
        } else {
            // emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RemoveInviteJob: Problem: "));
        }
#if 0
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
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

