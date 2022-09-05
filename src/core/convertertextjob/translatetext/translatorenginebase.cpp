/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorenginebase.h"
#include <KLocalizedString>

TranslatorEngineBase::TranslatorEngineBase(QObject *parent)
    : QObject{parent}
{
    mDebug = !qEnvironmentVariableIsEmpty("RUQOLA_TRANSLATION_DEBUGGING");
}

TranslatorEngineBase::~TranslatorEngineBase() = default;

void TranslatorEngineBase::setInputText(const QString &text)
{
    mInputText = text;
}

void TranslatorEngineBase::setFrom(const QString &language)
{
    mFrom = language;
}

void TranslatorEngineBase::setTo(const QString &language)
{
    mTo = language;
}

QString TranslatorEngineBase::resultTranslate() const
{
    return mResult;
}

QString TranslatorEngineBase::jsonDebug() const
{
    if (mDebug) {
        return mJsonDebug;
    } else {
        return {};
    }
}

void TranslatorEngineBase::setServerUrl(const QString &newServerUrl)
{
    mServerUrl = newServerUrl;
}

const QString &TranslatorEngineBase::apiKey() const
{
    return mApiKey;
}

void TranslatorEngineBase::setApiKey(const QString &newApiKey)
{
    mApiKey = newApiKey;
}

void TranslatorEngineBase::loadSettings()
{
    // TODO
    // Nothing here
}

void TranslatorEngineBase::clear()
{
    mJsonData.clear();
}

void TranslatorEngineBase::slotError(QNetworkReply::NetworkError error)
{
    QString messageError;
    if (error == QNetworkReply::ServiceUnavailableError) {
        messageError = i18n("Error: Engine systems have detected suspicious traffic from your computer network. Please try your request again later.");
    }
    Q_EMIT translateFailed(false, messageError);
}
