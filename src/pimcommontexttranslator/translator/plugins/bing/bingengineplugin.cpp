/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "bingengineplugin.h"
#include "translator/plugins/bing/bingtranslator_debug.h"
#include <KLocalizedString>
#include <PimCommonTextTranslator/TranslatorEngineAccessManager>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

QByteArray BingEnginePlugin::sBingKey;
QByteArray BingEnginePlugin::sBingToken;
QString BingEnginePlugin::sBingIg;
QString BingEnginePlugin::sBingIid;

BingEnginePlugin::BingEnginePlugin(QObject *parent)
    : PimCommonTextTranslator::TranslatorEnginePlugin(parent)
{
}

BingEnginePlugin::~BingEnginePlugin() = default;

void BingEnginePlugin::translate()
{
    if (sBingKey.isEmpty() || sBingToken.isEmpty()) {
        const QUrl url(QStringLiteral("https://www.bing.com/translator"));
        QNetworkReply *reply = PimCommonTextTranslator::TranslatorEngineAccessManager::self()->networkManager()->get(QNetworkRequest(url));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            parseCredentials(reply);
        });
        connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
            slotError(error);
            reply->deleteLater();
        });
    } else {
        translateText();
    }
}

void BingEnginePlugin::parseCredentials(QNetworkReply *reply)
{
    const QByteArray webSiteData = reply->readAll();
    reply->deleteLater();
    const QByteArray credentialsBeginString = QByteArrayLiteral("var params_RichTranslateHelper = [");
    const int credentialsBeginPos = webSiteData.indexOf(credentialsBeginString);

    if (credentialsBeginPos == -1) {
        Q_EMIT translateFailed(false, i18n("Error: Unable to find Bing credentials in web version."));
        return;
    }

    const int keyBeginPos = credentialsBeginPos + credentialsBeginString.size();
    const int keyEndPos = webSiteData.indexOf(',', keyBeginPos);

    if (keyEndPos == -1) {
        Q_EMIT translateFailed(false, i18n("Error: Unable to extract Bing key from web version."));
        return;
    }

    sBingKey = webSiteData.mid(keyBeginPos, keyEndPos - keyBeginPos);
    const int tokenBeginPos = keyEndPos + 2; // Skip two symbols instead of one because the value is enclosed in quotes
    const int tokenEndPos = webSiteData.indexOf('"', tokenBeginPos);

    if (tokenEndPos == -1) {
        Q_EMIT translateFailed(false, i18n("Error: Unable to extract Bing token from web version."));
        return;
    }

    sBingToken = webSiteData.mid(tokenBeginPos, tokenEndPos - tokenBeginPos);
    const int igBeginPos = webSiteData.indexOf("IG");
    const int igEndPos = webSiteData.indexOf('"', igBeginPos + 2);

    if (igEndPos == -1) {
        Q_EMIT translateFailed(false, i18n("Error: Unable to extract additional Bing information from web version."));
        return;
    }

    sBingIg = QString::fromUtf8(webSiteData.mid(igBeginPos, igEndPos - igBeginPos));
    const int iidBeginPos = webSiteData.indexOf("data-iid");
    const int iidEndPos = webSiteData.indexOf('"', iidBeginPos + 2);

    if (iidEndPos == -1) {
        Q_EMIT translateFailed(false, i18n("Error: Unable to extract additional Bing information from web version."));
        return;
    }

    sBingIid = QString::fromUtf8(webSiteData.mid(iidBeginPos, iidEndPos - iidBeginPos));

    // qCDebug(TRANSLATOR_bing) << "sBingIid " << sBingIid << " sBingIg " << sBingIg << " sBingToken " << sBingToken << " sBingKey " << sBingKey;
    translateText();
}

void BingEnginePlugin::translateText()
{
    if (verifyFromAndToLanguage()) {
        return;
    }
    clear();

    const QByteArray postData =
        "&text=" + QUrl::toPercentEncoding(inputText()) + "&fromLang=" + from().toUtf8() + "&to=" + to().toUtf8() + "&token=" + sBingToken + "&key=" + sBingKey;

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("IG"), sBingIg);
    urlQuery.addQueryItem(QStringLiteral("IDD"), sBingIid);
    QUrl url(QStringLiteral("https://www.bing.com/ttranslatev3"));
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QStringLiteral("%1/%2").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion()));

    QNetworkReply *reply = PimCommonTextTranslator::TranslatorEngineAccessManager::self()->networkManager()->post(request, postData);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        parseTranslation(reply);
    });
}

void BingEnginePlugin::parseTranslation(QNetworkReply *reply)
{
    // Parse translation data
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    qCDebug(TRANSLATOR_bing) << " jsonResponse " << jsonResponse;
    const QJsonObject responseObject = jsonResponse.array().first().toObject();
    if (from() == QStringLiteral("auto")) {
        const QString langCode = responseObject.value(QStringLiteral("detectedLanguage")).toObject().value(QStringLiteral("language")).toString();
        setFrom(langCode);
        //        if (m_sourceLang == NoLanguage)
        //        {
        //            resetData(ParsingError, i18n("Error: Unable to parse autodetected language"));
        //            return;
        //        }
    }

    const QJsonObject translationsObject = responseObject.value(QStringLiteral("translations")).toArray().first().toObject();
    appendResult(translationsObject.value(QStringLiteral("text")).toString());
    if (hasDebug()) {
        setJsonDebug(QString::fromUtf8(jsonResponse.toJson(QJsonDocument::Indented)));
    }

    qCDebug(TRANSLATOR_bing) << " mResult " << result();
    // m_translationTranslit               += translationsObject.value(QStringLiteral("transliteration")).toObject().value(QStringLiteral("text")).toString();
    reply->deleteLater();
    Q_EMIT translateDone();
}
