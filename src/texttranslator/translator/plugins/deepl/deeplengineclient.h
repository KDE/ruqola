/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <TextTranslator/TranslatorEngineClient>

class DeeplEngineClient : public TextTranslator::TranslatorEngineClient
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.Translator.deepl")
    Q_INTERFACES(TextTranslator::TranslatorEngineClient)
public:
    explicit DeeplEngineClient(QObject *parent = nullptr);
    ~DeeplEngineClient() override;

    Q_REQUIRED_RESULT QString name() const override;
    Q_REQUIRED_RESULT QString translatedName() const override;
    Q_REQUIRED_RESULT TextTranslator::TranslatorEnginePlugin *createTranslator() override;
    Q_REQUIRED_RESULT QMap<TextTranslator::TranslatorUtil::Language, QString> supportedLanguages() override;
    Q_REQUIRED_RESULT bool hasConfigurationDialog() const override;
    void showConfigureDialog(QWidget *parentWidget) override;

protected:
    Q_REQUIRED_RESULT bool isSupported(TextTranslator::TranslatorUtil::Language lang) const override;
};
