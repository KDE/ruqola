/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <TextTranslator/TranslatorEngineClient>

class BingEngineClient : public TextTranslator::TranslatorEngineClient
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.Translator.bing")
    Q_INTERFACES(TextTranslator::TranslatorEngineClient)
public:
    explicit BingEngineClient(QObject *parent = nullptr);
    ~BingEngineClient() override;

    Q_REQUIRED_RESULT QString name() const override;
    Q_REQUIRED_RESULT QString translatedName() const override;
    Q_REQUIRED_RESULT TextTranslator::TranslatorEnginePlugin *createTranslator() override;
    Q_REQUIRED_RESULT QMap<TextTranslator::TranslatorUtil::Language, QString> supportedLanguages() override;

protected:
    Q_REQUIRED_RESULT bool isSupported(TextTranslator::TranslatorUtil::Language lang) const override;
};
