/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineplugin.h"
#include "deeplengineutil.h"
#include "deepltranslator_debug.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <PimCommonTextTranslator/TranslatorEngineAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

DeeplEnginePlugin::DeeplEnginePlugin(QObject *parent)
    : PimCommonTextTranslator::TranslatorEnginePlugin(parent)
{
    loadSettings();
}

DeeplEnginePlugin::~DeeplEnginePlugin() = default;

void DeeplEnginePlugin::translate()
{
    if (verifyFromAndToLanguage()) {
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
    query.addQueryItem(QStringLiteral("target_lang"), to());
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    QNetworkReply *reply = PimCommonTextTranslator::TranslatorEngineAccessManager::self()->networkManager()->post(request, QByteArray());
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
    setResult(responseObject.value(QStringLiteral("translation")).toString());
    reply->deleteLater();
    qCDebug(TRANSLATOR_DEEPL_LOG) << " result " << result();
    Q_EMIT translateDone();
}

void DeeplEnginePlugin::loadSettings()
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), DeeplEngineUtil::groupName());
    mUseFreeLicense = myGroup.readEntry(DeeplEngineUtil::freeLicenseKey(), false);
    mServerUrl = mUseFreeLicense ? QStringLiteral("https://api-free.deepl.com/v2/translate") : QStringLiteral("https://api.deepl.com/v2/translate");
    // TODO load API key ? stored in kwallet ?
}

void DeeplEnginePlugin::slotConfigureChanged()
{
    loadSettings();
}
