/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deepltranslator.h"
#include "convertertextjob/translatetext/translatorengineaccessmanager.h"
#include "convertertextjob/translatetext/translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>

DeepLTranslator::DeepLTranslator(QObject *parent)
    : TranslatorEngineBase{parent}
{
}

DeepLTranslator::~DeepLTranslator() = default;

QString DeepLTranslator::engineName() const
{
    return i18n("DeepL");
}

QString DeepLTranslator::apiUrl() const
{
    return mServerUrl;
}

void DeepLTranslator::translate()
{
    if (mFrom == mTo) {
        Q_EMIT translateFailed(false, i18n("You used same language for from and to language."));
        return;
    }
    translateText();
}

QVector<QPair<QString, QString>> DeepLTranslator::supportedLanguage() const
{
    return languages();
}

void DeepLTranslator::translateText()
{
    mResult.clear();

    const QUrl url(QString::fromUtf8("%1/api/v1/%2/%3/%4").arg(mServerUrl).arg(mFrom).arg(mTo).arg(QString::fromUtf8(QUrl::toPercentEncoding(mInputText))));

    const QNetworkRequest request(url);

    QNetworkReply *reply = TranslatorEngineAccessManager::self()->networkManager()->get(request);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        slotError(error);
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        parseTranslation(reply);
    });
}

void DeepLTranslator::parseTranslation(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT translateFailed(false, reply->errorString());
        reply->deleteLater();
        return;
    }

    const QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    const QJsonObject responseObject = jsonResponse.object();
    mResult = responseObject.value(QStringLiteral("translation")).toString();
    reply->deleteLater();
    Q_EMIT translateDone();
}

void DeepLTranslator::loadSettings()
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), QStringLiteral("DeepLTranslator"));
    mUseFreeLicense = myGroup.readEntry(QStringLiteral("freeLicense"), false);
    mServerUrl = mUseFreeLicense ? QStringLiteral("https://api-free.deepl.com/v2/translate") : QStringLiteral("https://api.deepl.com/v2/translate");
    // TODO load API key ? stored in kwallet ?
}

QVector<QPair<QString, QString>> DeepLTranslator::languages()
{
    if (sSupportedLanguage.isEmpty()) {
        // TODO
    }
    return sSupportedLanguage;
}
