/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaengineplugin.h"

#include <PimCommonTextTranslator/TranslatorEngineAccessManager>

#include <KConfigGroup>
#include <KSharedConfig>

#include <QJsonDocument>
#include <QJsonObject>

LingvaEnginePlugin::LingvaEnginePlugin(QObject *parent)
    : PimCommonTextTranslator::TranslatorEnginePlugin(parent)
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

    const QUrl url(QStringLiteral("%1/api/v1/%2/%3/%4").arg(mServerUrl, from(), to(), QString::fromUtf8(QUrl::toPercentEncoding(inputText()))));

    const QNetworkRequest request(url);

    QNetworkReply *reply = PimCommonTextTranslator::TranslatorEngineAccessManager::self()->networkManager()->get(request);
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
    Q_EMIT translateDone();
}

void LingvaEnginePlugin::loadSettings()
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), QStringLiteral("LingvaTranslator"));
    mServerUrl = myGroup.readEntry(QStringLiteral("ServerUrl"), QStringLiteral("https://lingva.ml"));
}
