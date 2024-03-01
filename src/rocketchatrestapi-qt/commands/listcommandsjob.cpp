/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listcommandsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ListCommandsJob::ListCommandsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ListCommandsJob::~ListCommandsJob() = default;

bool ListCommandsJob::requireHttpAuthentication() const
{
    return true;
}

bool ListCommandsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start owninfo job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("ListCommandsJob: Ask info about me"));
    return true;
}

void ListCommandsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ListCommandsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT listCommandsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ListCommandsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest ListCommandsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CommandsList);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_listcommandsjob.cpp"
