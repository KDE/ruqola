/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanager/restauthenticationmanager.h"
#include "connection.h"
#include "misc/methodcalljob.h"
#include "ruqola_restapi_authentication_debug.h"
#include <QJsonArray>
// We use method.callAnon here.

RESTAuthenticationManager::RESTAuthenticationManager(Connection *restApiConnection, QObject *parent)
    : AuthenticationManagerBase(parent)
    , mRestApiConnection(restApiConnection)
{
}

RESTAuthenticationManager::~RESTAuthenticationManager() = default;

QJsonObject RESTAuthenticationManager::generateJsonMethod(const QString &method, const QJsonDocument &params, quint64 id)
{
    QJsonObject json;
    json[QLatin1StringView("msg")] = QStringLiteral("method");
    json[QLatin1StringView("method")] = method;
    json[QLatin1StringView("id")] = QString::number(id);

    if (!params.isEmpty()) {
        if (params.isArray()) {
            json[QLatin1StringView("params")] = params.array();
        } else if (params.isObject()) {
            QJsonArray arr;
            arr.append(params.object());
            json[QLatin1StringView("params")] = arr;
        }
    }
    return json;
}

void RESTAuthenticationManager::callLoginImpl(const QJsonArray &params, AuthenticationManagerBase::Method method)
{
    const QString methodName = AuthenticationManagerBase::convertMethodEnumToString(method);
    auto job = new RocketChatRestApi::MethodCallJob(this);
    RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
    info.methodName = methodName;
    info.anonymous = true;
    // TODO: verify mIndex
    info.messageObj = generateJsonMethod(info.methodName, QJsonDocument(params), mIndex++);
    job->setMethodCallJobInfo(std::move(info));
    mRestApiConnection->initializeRestApiJob(job);
    // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this, method](const QJsonObject &replyObject) {
        processMethodResponseImpl(replyObject, method);
    });

    if (!job->start()) {
        qCWarning(RUQOLA_RESTAPI_AUTH_LOG) << "Impossible to start MethodCallJob::login job";
    }
}

#include "moc_restauthenticationmanager.cpp"
