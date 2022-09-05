/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getsupportedlanguagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetSupportedLanguagesJob::GetSupportedLanguagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetSupportedLanguagesJob::~GetSupportedLanguagesJob() = default;

bool GetSupportedLanguagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetSupportedLanguagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetSupportedLanguagesJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("GetSupportedLanguagesJob: get message starting"));
    return true;
}

void GetSupportedLanguagesJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetSupportedLanguagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getSupportedLanguagesDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("GetSupportedLanguagesJob: Problem when we tried to GetSupportedLanguages : ")
                         + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GetSupportedLanguagesJob::needTargetLanguage() const
{
    return mNeedTargetLanguage;
}

void GetSupportedLanguagesJob::setNeedTargetLanguage(bool newNeedTargetLanguage)
{
    mNeedTargetLanguage = newNeedTargetLanguage;
}

QNetworkRequest GetSupportedLanguagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateGetSupportedLanguages);
    if (mNeedTargetLanguage) {
        QUrlQuery queryUrl;
        queryUrl.addQueryItem(QStringLiteral("targetLanguage"), QStringLiteral("en"));
        url.setQuery(queryUrl);
    }
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool GetSupportedLanguagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QString GetSupportedLanguagesJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("invalid-params")) {
        return i18n("Invalid Parameters");
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}
