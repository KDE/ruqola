/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later

  based on digikam onlinetranslator code
*/

#include "deeplengineplugin.h"
#include "deeplengineutil.h"
#include "deepltranslator_debug.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <TextTranslator/TranslatorEngineAccessManager>

DeeplEnginePlugin::DeeplEnginePlugin(QObject *parent)
    : TextTranslator::TranslatorEnginePlugin(parent)
{
    loadSettings();
}

DeeplEnginePlugin::~DeeplEnginePlugin() = default;

void DeeplEnginePlugin::translate()
{
    if (verifyFromAndToLanguage()) {
        return;
    }
    if (mApiKey.isEmpty()) {
        Q_EMIT translateFailed(false, i18n("Server needs Api Key."));
        return;
    }
    translateText();
}

void DeeplEnginePlugin::translateText()
{
    clear();

    QUrl url(mServerUrl);
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("text"), inputText());
    query.addQueryItem(QStringLiteral("source_lang"), languageCode(from()));
    query.addQueryItem(QStringLiteral("target_lang"), languageCode(to()));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("Authorization"), "DeepL-Auth-Key " + mApiKey.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    QNetworkReply *reply = TextTranslator::TranslatorEngineAccessManager::self()->networkManager()->post(request, QByteArray());
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        parseTranslation(reply);
    });
}

void DeeplEnginePlugin::parseTranslation(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT translateFailed(false, reply->errorString());
        reply->deleteLater();
        return;
    }

    const QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    if (hasDebug()) {
        setJsonDebug(QString::fromUtf8(jsonResponse.toJson(QJsonDocument::Indented)));
    }
    const QJsonObject responseObject = jsonResponse.object();
    // QJsonObject({"translations":[{"detected_source_language":"EN","text":"Bonjour le monde"}]})
    qCDebug(TRANSLATOR_DEEPL_LOG) << " responseObject " << responseObject;
    const QJsonArray arrayTranslation = responseObject.value(QStringLiteral("translations")).toArray();
    qCDebug(TRANSLATOR_DEEPL_LOG) << " arrayTranslation " << arrayTranslation;
    const QJsonObject obj = arrayTranslation.at(0).toObject();

    setResult(obj.value(QStringLiteral("text")).toString());
    reply->deleteLater();
    qCDebug(TRANSLATOR_DEEPL_LOG) << " result " << result();
    Q_EMIT translateDone();
}

void DeeplEnginePlugin::loadSettings()
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), DeeplEngineUtil::groupName());
    mUseFreeLicense = myGroup.readEntry(DeeplEngineUtil::freeLicenseKey(), false);
    mServerUrl = mUseFreeLicense ? QStringLiteral("https://api-free.deepl.com/v2/translate") : QStringLiteral("https://api.deepl.com/v2/translate");
    // TODO stored in kwallet ?
    mApiKey = myGroup.readEntry(DeeplEngineUtil::apiGroupName(), QString());
}

void DeeplEnginePlugin::slotConfigureChanged()
{
    loadSettings();
}

QString DeeplEnginePlugin::languageCode(const QString &langStr)
{
    if (langStr == QLatin1String("auto")) {
        return QStringLiteral("auto-detect");
    } else if (langStr == QLatin1String("zh-CN")) {
        return QStringLiteral("zh");
    }
    return langStr;
}
