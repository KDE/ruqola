/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  Code based on Digikam donlinetranslator
*/

#include "yandextranslator.h"
#include "translator/misc/translatorutil.h"
#include "translator/translatorengineaccessmanager.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace PimCommonTextTranslator;
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

QVector<QPair<QString, QString>> YandexTranslator::languages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = TranslatorUtil::genericLanguages();
        mLanguages += TranslatorUtil::yandexSpecificLanguages();
    }
    return mLanguages;
}

QVector<QPair<QString, QString>> YandexTranslator::supportedLanguage()
{
    return languages();
}

QString YandexTranslator::engineName() const
{
    return i18n("Yandex");
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
    // qCDebug(PIMCOMMON_LOG) << "webSiteData  " << webSiteData;
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

    mResult += jsonData.value(QStringLiteral("text")).toArray().at(0).toString();
    Q_EMIT translateDone();
}
