/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "googleengineclient.h"
#include "googleengineplugin.h"
#include "translator/misc/translatorutil.h"
#include <KLocalizedString>

GoogleEngineClient::GoogleEngineClient(QObject *parent)
    : TextTranslator::TranslatorEngineClient{parent}
{
}

GoogleEngineClient::~GoogleEngineClient() = default;

QString GoogleEngineClient::name() const
{
    return QStringLiteral("google");
}

QString GoogleEngineClient::translatedName() const
{
    return i18n("Google");
}

TextTranslator::TranslatorEnginePlugin *GoogleEngineClient::createTranslator()
{
    return new GoogleEnginePlugin();
}

QMap<TextTranslator::TranslatorUtil::Language, QString> GoogleEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = fillLanguages();
    }
    return mLanguages;
}

bool GoogleEngineClient::isSupported(TextTranslator::TranslatorUtil::Language lang) const
{
    switch (lang) {
    case TextTranslator::TranslatorUtil::ka:
    case TextTranslator::TranslatorUtil::tl:
        return false;
    default:
        break;
    }
    return true;
}
