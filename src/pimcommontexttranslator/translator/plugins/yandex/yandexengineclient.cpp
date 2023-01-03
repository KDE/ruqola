/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "yandexengineclient.h"
#include "translator/misc/translatorutil.h"
#include "yandexengineplugin.h"
#include <KLocalizedString>

YandexEngineClient::YandexEngineClient(QObject *parent)
    : PimCommonTextTranslator::TranslatorEngineClient{parent}
{
}

YandexEngineClient::~YandexEngineClient() = default;

QString YandexEngineClient::name() const
{
    return QStringLiteral("yandex");
}

QString YandexEngineClient::translatedName() const
{
    return i18n("Yandex");
}

PimCommonTextTranslator::TranslatorEnginePlugin *YandexEngineClient::createTranslator()
{
    return new YandexEnginePlugin();
}

QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> YandexEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = fillLanguages();
    }
    return mLanguages;
}

bool YandexEngineClient::isSupported(PimCommonTextTranslator::TranslatorUtil::Language lang) const
{
    switch (lang) {
    case PimCommonTextTranslator::TranslatorUtil::tl:
        return false;
    default:
        break;
    }
    return true;
}
