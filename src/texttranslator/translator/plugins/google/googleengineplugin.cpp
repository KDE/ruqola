/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "googleengineplugin.h"
#include "googletranslator_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <TextTranslator/TranslatorEngineAccessManager>

GoogleEnginePlugin::GoogleEnginePlugin(QObject *parent)
    : TextTranslator::TranslatorEnginePlugin(parent)
{
    connect(TextTranslator::TranslatorEngineAccessManager::self()->networkManager(),
            &QNetworkAccessManager::finished,
            this,
            &GoogleEnginePlugin::slotTranslateFinished);
}

GoogleEnginePlugin::~GoogleEnginePlugin() = default;

void GoogleEnginePlugin::translate()
{
    if (verifyFromAndToLanguage()) {
        return;
    }

    clear();

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("client"), QStringLiteral("gtx"));
    urlQuery.addQueryItem(QStringLiteral("sl"), languageCode(from()));
    urlQuery.addQueryItem(QStringLiteral("tl"), languageCode(to()));
    urlQuery.addQueryItem(QStringLiteral("dt"), QStringLiteral("t"));
    urlQuery.addQueryItem(QStringLiteral("q"), inputText());

    QUrl url;
    url.setQuery(urlQuery);
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("translate.googleapis.com"));
    url.setPath(QStringLiteral("/translate_a/single"));
    const QNetworkRequest request(url);

    QNetworkReply *reply = TextTranslator::TranslatorEngineAccessManager::self()->networkManager()->get(request);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });
}

QString GoogleEnginePlugin::languageCode(const QString &langStr)
{
    if (langStr == QLatin1String("iw")) {
        return QStringLiteral("iw");
    }
    return langStr;
}

void GoogleEnginePlugin::slotTranslateFinished(QNetworkReply *reply)
{
    clear();
    mJsonData = QString::fromUtf8(reply->readAll());
    reply->deleteLater();
    //  jsonData contains arrays like this: ["foo",,"bar"]
    //  but this is not valid JSON for QJSON, it expects empty strings: ["foo","","bar"]
    mJsonData.replace(QRegularExpression(QStringLiteral(",{3,3}")), QStringLiteral(",\"\",\"\","));
    mJsonData.replace(QRegularExpression(QStringLiteral(",{2,2}")), QStringLiteral(",\"\","));
    qCDebug(TRANSLATOR_GOOGLE_LOG) << mJsonData;

    QJsonParseError parsingError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(mJsonData.toUtf8(), &parsingError);
    if (parsingError.error != QJsonParseError::NoError || jsonDoc.isNull()) {
        Q_EMIT translateFailed(false);
        return;
    }
    const QVariantList json = jsonDoc.toVariant().toList();
    if (hasDebug()) {
        setJsonDebug(QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Indented)));
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
            appendResult(level1.toList().at(0).toString());
        }
    }
    Q_EMIT translateDone();
}
