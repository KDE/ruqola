/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bingtranslator.h"
#include "convertertextjob/translatetext/translatorengineaccessmanager.h"
#include "convertertextjob/translatetext/translatorutil.h"

#include <KLocalizedString>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

QByteArray BingTranslator::sBingKey;
QByteArray BingTranslator::sBingToken;
QString BingTranslator::sBingIg;
QString BingTranslator::sBingIid;

BingTranslator::BingTranslator(QObject *parent)
    : TranslatorEngineBase{parent}
{
}

BingTranslator::~BingTranslator() = default;

void BingTranslator::translate()
{
    qDebug() << " BingTranslator::translate() ";
    if (sBingKey.isEmpty() || sBingToken.isEmpty()) {
        const QUrl url(QStringLiteral("https://www.bing.com/translator"));
        QNetworkReply *reply = TranslatorEngineAccessManager::self()->networkManager()->get(QNetworkRequest(url));
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

QString BingTranslator::engineName() const
{
    return i18n("Bing");
}

void BingTranslator::parseCredentials(QNetworkReply *reply)
{
    qDebug() << " void BingTranslator::parseCredentials(QNetworkReply *reply)";
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

    qDebug() << "sBingIid " << sBingIid << " sBingIg " << sBingIg << " sBingToken " << sBingToken << " sBingKey " << sBingKey;
    translateText();
}

void BingTranslator::translateText()
{
    if (mFrom == mTo) {
        Q_EMIT translateFailed(false, i18n("You used same language for from and to language."));
        return;
    }

    mResult.clear();

    const QByteArray postData =
        "&text=" + QUrl::toPercentEncoding(mInputText) + "&fromLang=" + mFrom.toUtf8() + "&to=" + mTo.toUtf8() + "&token=" + sBingToken + "&key=" + sBingKey;

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("IG"), sBingIg);
    urlQuery.addQueryItem(QStringLiteral("IDD"), sBingIid);
    QUrl url(QStringLiteral("https://www.bing.com/ttranslatev3"));
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QString::fromUtf8("%1/%2").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion()));

    qDebug() << " url " << url;
    QNetworkReply *reply = TranslatorEngineAccessManager::self()->networkManager()->post(request, postData);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        parseTranslation(reply);
    });
}

void BingTranslator::parseTranslation(QNetworkReply *reply)
{
    qDebug() << " void BingTranslator::parseTranslation(QNetworkReply *reply)";
    // Parse translation data
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    qDebug() << " jsonResponse " << jsonResponse;
    const QJsonObject responseObject = jsonResponse.array().first().toObject();
    if (mFrom == QStringLiteral("auto")) {
        const QString langCode = responseObject.value(QStringLiteral("detectedLanguage")).toObject().value(QStringLiteral("language")).toString();
        mFrom = langCode;
        //        if (m_sourceLang == NoLanguage)
        //        {
        //            resetData(ParsingError, i18n("Error: Unable to parse autodetected language"));
        //            return;
        //        }
    }

    const QJsonObject translationsObject = responseObject.value(QStringLiteral("translations")).toArray().first().toObject();
    mResult += translationsObject.value(QStringLiteral("text")).toString();
    qDebug() << " mResult " << mResult;
    // m_translationTranslit               += translationsObject.value(QStringLiteral("transliteration")).toObject().value(QStringLiteral("text")).toString();
    reply->deleteLater();
    Q_EMIT translateDone();
}

QVector<QPair<QString, QString>> BingTranslator::supportedLanguage() const
{
    checkLoadedSupportedLanguage();
    return languages();
}

QVector<QPair<QString, QString>> BingTranslator::languages()
{
    TranslatorUtil translatorUtil;
    QVector<QPair<QString, QString>> fullListLanguage;
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::automatic));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::en));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::nl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::fr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::de));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::el));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::it));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ja));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ko));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::pt));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ru));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::es));

    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::af));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sq));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ar));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hy));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::az));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::eu));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::be));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::bg));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ca));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::cs));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::da));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::et));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::tl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::fi));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::gl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ka));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ht));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::iw));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hi));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hu));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::is));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::id));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ga));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::lv));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::lt));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::mk));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ms));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::mt));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::no));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::fa));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::pl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ro));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sk));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sw));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sv));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::th));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::tr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::uk));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ur));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::vi));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::cy));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::yi));
    return fullListLanguage;
}
