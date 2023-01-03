/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimcommontexttranslator_export.h"
#include <PimCommonTextTranslator/TranslatorUtil>
#include <QObject>
#include <memory>
namespace PimCommonTextTranslator
{
class TranslatorEngineLoaderPrivate;
class TranslatorEngineClient;
class PIMCOMMONTEXTTRANSLATOR_EXPORT TranslatorEngineLoader : public QObject
{
    Q_OBJECT
public:
    static TranslatorEngineLoader *self();

    explicit TranslatorEngineLoader(QObject *parent = nullptr);
    ~TranslatorEngineLoader() override;

    Q_REQUIRED_RESULT PimCommonTextTranslator::TranslatorEngineClient *createTranslatorClient(const QString &clientName);

    // engine name, i18n
    Q_REQUIRED_RESULT QMap<QString, QString> translatorEngineInfos() const;

    Q_REQUIRED_RESULT QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> supportedLanguages(const QString &clientName) const;

    Q_REQUIRED_RESULT bool hasConfigurationDialog(const QString &clientName) const;

    Q_REQUIRED_RESULT QString fallbackFirstEngine() const;

    void showConfigureDialog(const QString &clientName, QWidget *parentWidget);

Q_SIGNALS:
    void loadingTranslatorFailed();

private:
    void loadPlugins();
    void loadPlugin(const QString &pluginPath);
    std::unique_ptr<TranslatorEngineLoaderPrivate> const d;
};
}
