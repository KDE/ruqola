/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later

  based on digikam onlinetranslator code
*/

#include "bingengineplugin.h"
#include "translator/plugins/bing/bingtranslator_debug.h"
#include <KLocalizedString>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <TextTranslator/TranslatorEngineAccessManager>

QByteArray BingEnginePlugin::sBingKey;
QByteArray BingEnginePlugin::sBingToken;
QString BingEnginePlugin::sBingIg;
QString BingEnginePlugin::sBingIid;

BingEnginePlugin::BingEnginePlugin(QObject *parent)
    : TextTranslator::TranslatorEnginePlugin(parent)
{
}

BingEnginePlugin::~BingEnginePlugin() = default;

void BingEnginePlugin::translate()
{
    if (sBingKey.isEmpty() || sBingToken.isEmpty()) {
        const QUrl url(QStringLiteral("https://www.bing.com/translator"));
        QNetworkReply *reply = TextTranslator::TranslatorEngineAccessManager::self()->networkManager()->get(QNetworkRequest(url));
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

    // qCDebug(TRANSLATOR_BING_LOG) << "sBingIid " << sBingIid << " sBingIg " << sBingIg << " sBingToken " << sBingToken << " sBingKey " << sBingKey;
    translateText();
}

void BingEnginePlugin::translateText()
{
    if (verifyFromAndToLanguage()) {
        return;
    }
    clear();

    const QByteArray postData = "&text=" + QUrl::toPercentEncoding(inputText()) + "&fromLang=" + languageCode(from()).toUtf8()
        + "&to=" + languageCode(to()).toUtf8() + "&token=" + sBingToken + "&key=" + sBingKey;

    qCDebug(TRANSLATOR_BING_LOG) << " postData " << postData;
    QUrl url(QStringLiteral("https://www.bing.com/ttranslatev3"));
    url.setQuery(QStringLiteral("IG=%1&IID=%2").arg(sBingIg, sBingIid));
    qCDebug(TRANSLATOR_BING_LOG) << " url " << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QStringLiteral("%1/%2").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion()));

    QNetworkReply *reply = TextTranslator::TranslatorEngineAccessManager::self()->networkManager()->post(request, postData);
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
    qCDebug(TRANSLATOR_BING_LOG) << " jsonResponse " << jsonResponse;
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

    qCDebug(TRANSLATOR_BING_LOG) << " mResult " << result();
    // m_translationTranslit               += translationsObject.value(QStringLiteral("transliteration")).toObject().value(QStringLiteral("text")).toString();
    reply->deleteLater();
    Q_EMIT translateDone();
}

QString BingEnginePlugin::languageCode(const QString &langStr)
{
    if (langStr == QLatin1String("auto")) {
        return QStringLiteral("auto-detect");
    } else if (langStr == QLatin1String("sr")) {
        return QStringLiteral("sr-Cyrl");
    } else if (langStr == QLatin1String("bs")) {
        return QStringLiteral("bs-Latn");
    } else if (langStr == QLatin1String("hmn")) {
        return QStringLiteral("mww");
    } else if (langStr == QLatin1String("zh")) {
        return QStringLiteral("zh-Hans");
    } else if (langStr == QLatin1String("zt")) {
        return QStringLiteral("zh-Hant");
    }
    return langStr;
}
