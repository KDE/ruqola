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

QVector<QPair<QString, QString>> YandexEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = PimCommonTextTranslator::TranslatorUtil::genericLanguages();
        mLanguages += PimCommonTextTranslator::TranslatorUtil::yandexSpecificLanguages();
    }
    return mLanguages;
}
