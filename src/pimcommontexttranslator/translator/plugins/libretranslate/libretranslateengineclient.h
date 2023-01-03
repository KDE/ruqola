/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <PimCommonTextTranslator/TranslatorEngineClient>

class LibreTranslateEngineClient : public PimCommonTextTranslator::TranslatorEngineClient
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.Translator.libretranslate")
    Q_INTERFACES(PimCommonTextTranslator::TranslatorEngineClient)
public:
    explicit LibreTranslateEngineClient(QObject *parent = nullptr);
    ~LibreTranslateEngineClient() override;

    Q_REQUIRED_RESULT QString name() const override;
    Q_REQUIRED_RESULT QString translatedName() const override;
    Q_REQUIRED_RESULT PimCommonTextTranslator::TranslatorEnginePlugin *createTranslator() override;
    Q_REQUIRED_RESULT QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> supportedLanguages() override;
    Q_REQUIRED_RESULT bool hasConfigurationDialog() const override;
    void showConfigureDialog(QWidget *parentWidget) override;

protected:
    Q_REQUIRED_RESULT bool isSupported(PimCommonTextTranslator::TranslatorUtil::Language lang) const override;
};
