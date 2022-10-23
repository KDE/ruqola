/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googletranslator.h"
#include "ruqola_translation_debug.h"
#include "translatetext/translatorengineaccessmanager.h"
#include "translatetext/translatorutil.h"
#include <KLocalizedString>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QPointer>
#include <QUrlQuery>

GoogleTranslator::GoogleTranslator(QObject *parent)
    : TranslatorEngineBase(parent)
{
    connect(TranslatorEngineAccessManager::self()->networkManager(), &QNetworkAccessManager::finished, this, &GoogleTranslator::slotTranslateFinished);
}

GoogleTranslator::~GoogleTranslator() = default;

QVector<QPair<QString, QString>> GoogleTranslator::supportedLanguage() const
{
    return languages();
}

QVector<QPair<QString, QString>> GoogleTranslator::languages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = TranslatorUtil::genericLanguages();
        mLanguages += TranslatorUtil::googleSpecificLanguages();
    }
    return mLanguages;
}

void GoogleTranslator::translate()
{
    if (mFrom == mTo) {
        Q_EMIT translateFailed(false, i18n("You used same language for from and to language."));
        return;
    }

    mResult.clear();

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("client"), QStringLiteral("gtx"));
    urlQuery.addQueryItem(QStringLiteral("sl"), mFrom);
    urlQuery.addQueryItem(QStringLiteral("tl"), mTo);
    urlQuery.addQueryItem(QStringLiteral("dt"), QStringLiteral("t"));
    urlQuery.addQueryItem(QStringLiteral("q"), mInputText);

    QUrl url;
    url.setQuery(urlQuery);
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("translate.googleapis.com"));
    url.setPath(QStringLiteral("/translate_a/single"));
    const QNetworkRequest request(url);

    QNetworkReply *reply = TranslatorEngineAccessManager::self()->networkManager()->get(request);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });
}

QString GoogleTranslator::engineName() const
{
    return i18n("Google");
}

void GoogleTranslator::slotTranslateFinished(QNetworkReply *reply)
{
    mResult.clear();
    mJsonData = QString::fromUtf8(reply->readAll());
    reply->deleteLater();
    //  jsonData contains arrays like this: ["foo",,"bar"]
    //  but this is not valid JSON for QJSON, it expects empty strings: ["foo","","bar"]
    mJsonData.replace(QRegularExpression(QStringLiteral(",{3,3}")), QStringLiteral(",\"\",\"\","));
    mJsonData.replace(QRegularExpression(QStringLiteral(",{2,2}")), QStringLiteral(",\"\","));
    qCDebug(RUQOLA_TRANSLATION_LOG) << mJsonData;

    QJsonParseError parsingError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(mJsonData.toUtf8(), &parsingError);
    if (parsingError.error != QJsonParseError::NoError || jsonDoc.isNull()) {
        Q_EMIT translateFailed(false);
        return;
    }
    const QVariantList json = jsonDoc.toVariant().toList();
    if (mDebug) {
        mJsonDebug = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Indented));
    }
    for (const QVariant &level0 : json) {
        const QVariantList listLevel0 = level0.toList();
        if (listLevel0.isEmpty()) {
            continue;
        }
        for (const QVariant &level1 : listLevel0) {
            if (level1.toList().size() <= 2) {
                continue;
            }
            mResult += level1.toList().at(0).toString();
        }
    }
    Q_EMIT translateDone();
}
