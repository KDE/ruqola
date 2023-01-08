/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later

  based on digikam onlinetranslator code

*/

#include "lingvaengineplugin.h"
#include "lingvaengineutil.h"
#include "lingvatranslator_debug.h"
#include <TextTranslator/TranslatorEngineAccessManager>

#include <KConfigGroup>
#include <KSharedConfig>

#include <QJsonDocument>
#include <QJsonObject>

LingvaEnginePlugin::LingvaEnginePlugin(QObject *parent)
    : TextTranslator::TranslatorEnginePlugin(parent)
{
    loadSettings();
}

LingvaEnginePlugin::~LingvaEnginePlugin() = default;

void LingvaEnginePlugin::translate()
{
    if (verifyFromAndToLanguage()) {
        return;
    }
    translateText();
}

void LingvaEnginePlugin::translateText()
{
    clear();

    const QUrl url(QStringLiteral("%1/api/v1/%2/%3/%4")
                       .arg(mServerUrl, languageCode(from()), languageCode(to()), QString::fromUtf8(QUrl::toPercentEncoding(inputText()))));

    qCDebug(TRANSLATOR_LINGVA_LOG) << " url " << url;
    const QNetworkRequest request(url);

    QNetworkReply *reply = TextTranslator::TranslatorEngineAccessManager::self()->networkManager()->get(request);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        parseTranslation(reply);
    });
}

void LingvaEnginePlugin::parseTranslation(QNetworkReply *reply)
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
    qCDebug(TRANSLATOR_LINGVA_LOG) << " result " << result();
    Q_EMIT translateDone();
}

void LingvaEnginePlugin::loadSettings()
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), LingvaEngineUtil::groupName());
    mServerUrl = myGroup.readEntry(LingvaEngineUtil::serverUrlKey(), LingvaEngineUtil::defaultServerUrl());
}

void LingvaEnginePlugin::slotConfigureChanged()
{
    loadSettings();
}

QString LingvaEnginePlugin::languageCode(const QString &langStr)
{
    if (langStr == QLatin1String("zh")) {
        return QStringLiteral("zh");
    } else if (langStr == QLatin1String("zt")) {
        return QStringLiteral("zh_HANT");
    }
    return langStr;
}
