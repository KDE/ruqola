/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "textedittexttospeech_export.h"
#include <TextEditTextToSpeech/AbstractTextToSpeechConfigInterface>

class QTextToSpeech;
namespace TextEditTextToSpeech
{
/**
 * @brief The TextToSpeechConfigInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechConfigInterface : public AbstractTextToSpeechConfigInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigInterface(QObject *parent = nullptr);
    ~TextToSpeechConfigInterface() override;

    Q_REQUIRED_RESULT QVector<QLocale> availableLocales() const override;
    Q_REQUIRED_RESULT QLocale locale() const override;

    Q_REQUIRED_RESULT QStringList availableEngines() const override;
    Q_REQUIRED_RESULT QStringList availableVoices() const override;
    void setEngine(const QString &engineName) override;
    void testEngine(const EngineSettings &engineSettings) override;
    QTextToSpeech *mTextToSpeech = nullptr;
};
}
