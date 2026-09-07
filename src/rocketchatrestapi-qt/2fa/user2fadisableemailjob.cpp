/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2fadisableemailjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
User2FADisableEmailJob::User2FADisableEmailJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

User2FADisableEmailJob::~User2FADisableEmailJob() = default;

bool User2FADisableEmailJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("User2FADisableEmailJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void User2FADisableEmailJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("User2FADisableEmailJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT disableEmailDone();
    }
}

bool User2FADisableEmailJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest User2FADisableEmailJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Users2FADisableEmail);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument User2FADisableEmailJob::json() const
{
    const QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool User2FADisableEmailJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_user2fadisableemailjob.cpp"
