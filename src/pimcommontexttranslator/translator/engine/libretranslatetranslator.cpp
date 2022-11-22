/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  Code based on Digikam donlinetranslator
*/

#include "libretranslatetranslator.h"
#include "translator/misc/translatorutil.h"
#include "translator/translatorengineaccessmanager.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>

using namespace PimCommonTextTranslator;
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

QVector<QPair<QString, QString>> LibreTranslateTranslator::supportedLanguage()
{
    return languages();
}

QVector<QPair<QString, QString>> LibreTranslateTranslator::languages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = TranslatorUtil::genericLanguages();
    }
    return mLanguages;
}

void LibreTranslateTranslator::translateText()
{
    mResult.clear();

    QByteArray postData = "&q=" + QUrl::toPercentEncoding(mInputText) + "&source=" + mFrom.toUtf8() + "&target=" + mTo.toUtf8();
    if (!mApiKey.isEmpty()) {
        postData += "&api_key=" + mApiKey.toUtf8();
    }
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));
    request.setUrl(QUrl(QStringLiteral("%1/translate").arg(mServerUrl)));

    QNetworkReply *reply = TranslatorEngineAccessManager::self()->networkManager()->post(request, postData);
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
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    const QJsonObject responseObject = jsonResponse.object();
    mResult = responseObject.value(QStringLiteral("translatedText")).toString();
    reply->deleteLater();
    Q_EMIT translateDone();
}

void LibreTranslateTranslator::loadSettings()
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), QStringLiteral("LibreTranslateTranslator"));
    mServerUrl = myGroup.readEntry(QStringLiteral("ServerUrl"), QString());
}
