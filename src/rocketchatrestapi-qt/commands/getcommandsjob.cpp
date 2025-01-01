/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getcommandsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
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
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("GetCommandsJob: Ask info about me"));
    return true;
}

void GetCommandsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetCommandsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getCommandsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetCommandsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
    addRequestAttribute(request);

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

#include "moc_getcommandsjob.cpp"
