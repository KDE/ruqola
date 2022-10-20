/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "libretranslatetranslator.h"
#include "translatetext/translatorengineaccessmanager.h"
#include "translatetext/translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QNetworkAccessManager>

LibreTranslateTranslator::LibreTranslateTranslator(QObject *parent)
    : TranslatorEngineBase{parent}
{
}

LibreTranslateTranslator::~LibreTranslateTranslator() = default;

QString LibreTranslateTranslator::engineName() const
{
    return i18n("Libre Translate");
}

void LibreTranslateTranslator::translate()
{
    if (mFrom == mTo) {
        Q_EMIT translateFailed(false, i18n("You used same language for from and to language."));
        return;
    }
    translateText();
}

QVector<QPair<QString, QString>> LibreTranslateTranslator::supportedLanguage() const
{
    return languages();
}

void LibreTranslateTranslator::translateText()
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

void LibreTranslateTranslator::parseTranslation(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT translateFailed(false, reply->errorString());
        reply->deleteLater();
        return;
    }

    //    const QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    //    const QJsonObject responseObject = jsonResponse.object();
    //    mResult = responseObject.value(QStringLiteral("translation")).toString();
    reply->deleteLater();
    Q_EMIT translateDone();
}

void LibreTranslateTranslator::loadSettings()
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), QStringLiteral("LibreTranslateTranslator"));
    mServerUrl = myGroup.readEntry(QStringLiteral("ServerUrl"), QStringLiteral("https://translate.argosopentech.com/"));
}

QVector<QPair<QString, QString>> LibreTranslateTranslator::languages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = TranslatorUtil::genericLanguages();
    }
    return mLanguages;
}
