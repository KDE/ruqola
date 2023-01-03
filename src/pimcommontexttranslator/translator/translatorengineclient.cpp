/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorengineclient.h"

using namespace PimCommonTextTranslator;

TranslatorEngineClient::TranslatorEngineClient(QObject *parent)
    : QObject{parent}
{
}

TranslatorEngineClient::~TranslatorEngineClient() = default;

bool TranslatorEngineClient::hasConfigurationDialog() const
{
    // False by default
    return false;
}

void TranslatorEngineClient::showConfigureDialog(QWidget *parentWidget)
{
    Q_UNUSED(parentWidget);
    // Nothing by default
}

QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> TranslatorEngineClient::fillLanguages()
{
    QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> map;
    for (int i = PimCommonTextTranslator::TranslatorUtil::Language::automatic; i < PimCommonTextTranslator::TranslatorUtil::Language::lastLanguage; ++i) {
        if (isSupported(static_cast<PimCommonTextTranslator::TranslatorUtil::Language>(i))) {
            map.insert(static_cast<PimCommonTextTranslator::TranslatorUtil::Language>(i),
                       PimCommonTextTranslator::TranslatorUtil::translatedLanguage(static_cast<PimCommonTextTranslator::TranslatorUtil::Language>(i)));
        }
    }
    return map;
}
