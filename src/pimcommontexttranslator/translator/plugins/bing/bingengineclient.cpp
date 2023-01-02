/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "bingengineclient.h"
#include "bingengineplugin.h"
#include "translator/misc/translatorutil.h"
#include <KLocalizedString>

BingEngineClient::BingEngineClient(QObject *parent)
    : PimCommonTextTranslator::TranslatorEngineClient{parent}
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

PimCommonTextTranslator::TranslatorEnginePlugin *BingEngineClient::createTranslator()
{
    return new BingEnginePlugin();
}

QVector<QPair<QString, QString>> BingEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = PimCommonTextTranslator::TranslatorUtil::genericLanguages();
#if 0
    { QLatin1String("bs-BG"),  QLatin1String("bs-Latn")  },         //                                            NOTE: Bing translator only
    { QLatin1String("sr-RS"),  QLatin1String("sr-Cyrl")  },         //                                            NOTE: Bing translator only
    { QLatin1String("zh-CN"),  QLatin1String("zh-Hans")  },         // SimplifiedChinese                        ; NOTE: Bing translator only
    { QLatin1String("zh-TW"),  QLatin1String("zh-Hant")  }          // TraditionalChinese                       ; NOTE: Bing translator only
#endif
    }
    return mLanguages;
}
