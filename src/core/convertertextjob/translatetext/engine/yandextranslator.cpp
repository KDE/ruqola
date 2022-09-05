/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "yandextranslator.h"
#include "convertertextjob/translatetext/translatorengineaccessmanager.h"
#include "convertertextjob/translatetext/translatorutil.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QString YandexTranslator::sYandexKey;
YandexTranslator::YandexTranslator(QObject *parent)
    : TranslatorEngineBase{parent}
{
}

YandexTranslator::~YandexTranslator() = default;

void YandexTranslator::translate()
{
    if (sYandexKey.isEmpty()) {
        const QUrl url(QStringLiteral("https://translate.yandex.com"));

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

QVector<QPair<QString, QString>> YandexTranslator::supportedLanguage() const
{
    return languages();
}

QString YandexTranslator::engineName() const
{
    return i18n("Yandex");
}

QVector<QPair<QString, QString>> YandexTranslator::languages()
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

void YandexTranslator::parseCredentials(QNetworkReply *reply)
{
    // Check availability of service
    const QByteArray webSiteData = reply->readAll();
    reply->deleteLater();
    if (webSiteData.isEmpty() || webSiteData.contains("<title>Oops!</title>") || webSiteData.contains("<title>302 Found</title>")) {
        Q_EMIT translateFailed(false,
                               i18n("Error: Engine systems have detected suspicious traffic "
                                    "from your computer network. Please try your request again later."));
        return;
    }
    qDebug() << "webSiteData  " << webSiteData;
    const QByteArray sidBeginString = "SID: '";
    const int sidBeginStringPos = webSiteData.indexOf(sidBeginString);

    if (sidBeginStringPos == -1) {
        Q_EMIT translateFailed(false, i18n("Error: Unable to find Yandex SID in web version."));
        return;
    }

    const int sidBeginPosition = sidBeginStringPos + sidBeginString.size();
    const int sidEndPosition = webSiteData.indexOf('\'', sidBeginPosition);

    if (sidEndPosition == -1) {
        Q_EMIT translateFailed(false, i18n("Error: Unable to extract Yandex SID from web version."));
        return;
    }

    // Yandex show reversed parts of session ID, need to decode

    const QString sid = QString::fromUtf8(webSiteData.mid(sidBeginPosition, sidEndPosition - sidBeginPosition));

    QStringList sidParts = sid.split(QLatin1Char('.'));

    for (int i = 0; i < sidParts.size(); ++i) {
        std::reverse(sidParts[i].begin(), sidParts[i].end());
    }

    sYandexKey = sidParts.join(QLatin1Char('.'));
    translateText();
}

void YandexTranslator::translateText()
{
    if (mFrom == mTo) {
        Q_EMIT translateFailed(false, i18n("You used same language for from and to language."));
        return;
    }

    mResult.clear();
    QString lang;
    if (mFrom == QStringLiteral("auto")) {
        lang = mTo;
    } else {
        lang = mFrom + QLatin1Char('-') + mTo;
    }
    // qDebug() << " lang " << lang;
    // Generate API url
    QUrl url(QStringLiteral("https://translate.yandex.net/api/v1/tr.json/translate"));
    url.setQuery(QStringLiteral("id=%1-2-0&srv=tr-text&text=%2&lang=%3").arg(sYandexKey, QString::fromUtf8(QUrl::toPercentEncoding(mInputText)), lang));

    // Setup request
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));
    request.setUrl(url);

    // Make reply
    QNetworkReply *reply = TranslatorEngineAccessManager::self()->networkManager()->post(request, QByteArray());
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        parseTranslation(reply);
    });
}

void YandexTranslator::parseTranslation(QNetworkReply *reply)
{
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    const QJsonObject jsonData = jsonResponse.object();
    reply->deleteLater();

    // Parse language
    if (mFrom == QStringLiteral("auto")) {
        QString sourceCode = jsonData.value(QStringLiteral("lang")).toString();
        sourceCode = sourceCode.left(sourceCode.indexOf(QLatin1Char('-')));
        // m_sourceLang       = language(Yandex, sourceCode);
#if 0
        if (m_sourceLang == NoLanguage)
        {
            Q_EMIT translateFailed(false, i18n("Error: Unable to parse autodetected language"));
            return;
        }
#endif
    }

    // Parse translation data
    mResult += jsonData.value(QStringLiteral("text")).toArray().at(0).toString();
    // qDebug() << " result " << reply->readAll();
    // TODO
    Q_EMIT translateDone();
}
