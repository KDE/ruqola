/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getsupportedlanguagesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
    addStartRestApiInfo("GetSupportedLanguagesJob: get message starting"_ba);
    return true;
}

void GetSupportedLanguagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetSupportedLanguagesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getSupportedLanguagesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetSupportedLanguagesJob: Problem when we tried to GetSupportedLanguages : "_ba + replyJson.toJson(QJsonDocument::Indented));
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
        queryUrl.addQueryItem(u"targetLanguage"_s, u"en"_s);
        url.setQuery(queryUrl);
    }
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString GetSupportedLanguagesJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == "invalid-params"_L1) {
        return i18n("Invalid Parameters");
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

#include "moc_getsupportedlanguagesjob.cpp"
