/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  Code based on Digikam donlinetranslator
*/

#pragma once

#include "translator/translatorenginebase.h"
namespace PimCommonTextTranslator
{
class LibreTranslateTranslator : public TranslatorEngineBase
{
    Q_OBJECT
public:
    explicit LibreTranslateTranslator(QObject *parent = nullptr);
    ~LibreTranslateTranslator() override;

    Q_REQUIRED_RESULT QString engineName() const override;
    void translate() override;
    Q_REQUIRED_RESULT QVector<QPair<QString, QString>> supportedLanguage() override;
    void loadSettings() override;
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> languages();

private:
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    static inline QVector<QPair<QString, QString>> mLanguages;
};
}
