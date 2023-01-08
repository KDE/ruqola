/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "bingengineclient.h"
#include "bingengineplugin.h"
#include "translator/misc/translatorutil.h"
#include <KLocalizedString>

BingEngineClient::BingEngineClient(QObject *parent)
    : TextTranslator::TranslatorEngineClient{parent}
{
}

BingEngineClient::~BingEngineClient() = default;

QString BingEngineClient::name() const
{
    return QStringLiteral("bing");
}

QString BingEngineClient::translatedName() const
{
    return i18n("Bing");
}

TextTranslator::TranslatorEnginePlugin *BingEngineClient::createTranslator()
{
    return new BingEnginePlugin();
}

QMap<TextTranslator::TranslatorUtil::Language, QString> BingEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = fillLanguages();
    }
    return mLanguages;
}

bool BingEngineClient::isSupported(TextTranslator::TranslatorUtil::Language lang) const
{
    switch (lang) {
    case TextTranslator::TranslatorUtil::sq:
    case TextTranslator::TranslatorUtil::hy:
    case TextTranslator::TranslatorUtil::az:
    case TextTranslator::TranslatorUtil::eu:
    case TextTranslator::TranslatorUtil::be:
    case TextTranslator::TranslatorUtil::gl:
    case TextTranslator::TranslatorUtil::ka:
    case TextTranslator::TranslatorUtil::ga:
    case TextTranslator::TranslatorUtil::mk:
    case TextTranslator::TranslatorUtil::yi:
        return false;
    default:
        break;
    }
    return true;
}
