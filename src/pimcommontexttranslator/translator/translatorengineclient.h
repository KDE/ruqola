/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include "pimcommontexttranslator_export.h"
#include <PimCommonTextTranslator/TranslatorUtil>
#include <QMap>
namespace PimCommonTextTranslator
{
class TranslatorEnginePlugin;
class PIMCOMMONTEXTTRANSLATOR_EXPORT TranslatorEngineClient : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorEngineClient(QObject *parent = nullptr);
    ~TranslatorEngineClient() override;

    /**
     * @returns the name of the implementing class.
     */
    virtual QString name() const = 0;

    virtual QString translatedName() const = 0;

    virtual TranslatorEnginePlugin *createTranslator() = 0;

    Q_REQUIRED_RESULT virtual QMap<TranslatorUtil::Language, QString> supportedLanguages() = 0;

    Q_REQUIRED_RESULT virtual bool hasConfigurationDialog() const;

    virtual void showConfigureDialog(QWidget *parentWidget);

Q_SIGNALS:
    void configureChanged();

protected:
    Q_REQUIRED_RESULT QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> fillLanguages();
    Q_REQUIRED_RESULT virtual bool isSupported(PimCommonTextTranslator::TranslatorUtil::Language lang) const = 0;
    QMap<TranslatorUtil::Language, QString> mLanguages;
};
}
Q_DECLARE_INTERFACE(PimCommonTextTranslator::TranslatorEngineClient, "org.kde.translator.Client")
