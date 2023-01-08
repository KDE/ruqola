/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorengineclient.h"

using namespace TextTranslator;

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

QMap<TextTranslator::TranslatorUtil::Language, QString> TranslatorEngineClient::fillLanguages()
{
    QMap<TextTranslator::TranslatorUtil::Language, QString> map;
    for (int i = TextTranslator::TranslatorUtil::Language::automatic; i < TextTranslator::TranslatorUtil::Language::lastLanguage; ++i) {
        if (isSupported(static_cast<TextTranslator::TranslatorUtil::Language>(i))) {
            map.insert(static_cast<TextTranslator::TranslatorUtil::Language>(i),
                       TextTranslator::TranslatorUtil::translatedLanguage(static_cast<TextTranslator::TranslatorUtil::Language>(i)));
        }
    }
    return map;
}
