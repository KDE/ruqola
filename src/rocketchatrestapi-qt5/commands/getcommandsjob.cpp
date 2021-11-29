/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "getcommandsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetCommandsJob::GetCommandsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetCommandsJob::~GetCommandsJob() = default;

bool GetCommandsJob::requireHttpAuthentication() const
{
    return true;
}

bool GetCommandsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start owninfo job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetCommandsJob::slotGetCommandsFinished);
    addStartRestApiInfo(QByteArrayLiteral("GetCommandsJob: Ask info about me"));
    return true;
}

void GetCommandsJob::slotGetCommandsFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetCommandsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getCommandsDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetCommandsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString GetCommandsJob::commandName() const
{
    return mCommandName;
}

void GetCommandsJob::setCommandName(const QString &commandName)
{
    mCommandName = commandName;
}

QNetworkRequest GetCommandsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CommandsGet);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("command"), mCommandName);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool GetCommandsJob::canStart() const
{
    if (mCommandName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetCommandsJob: commandName is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}
