/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textedittexttospeech_export.h"
#include <QLocale>
#include <QObject>
#include <QVector>
namespace TextEditTextToSpeech
{
/**
 * @brief The AbstractTextToSpeechConfigInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEDITTEXTTOSPEECH_EXPORT AbstractTextToSpeechConfigInterface : public QObject
{
    Q_OBJECT
public:
    struct EngineSettings {
        int rate = 0;
        int pitch = 0;
        int volume = 0;
        QString voice;
        QString localeName;
    };
    explicit AbstractTextToSpeechConfigInterface(QObject *parent = nullptr);
    ~AbstractTextToSpeechConfigInterface() override;
    Q_REQUIRED_RESULT virtual QVector<QLocale> availableLocales() const;
    Q_REQUIRED_RESULT virtual QLocale locale() const;
    virtual void setLocale(const QLocale &locale);

    Q_REQUIRED_RESULT virtual QStringList availableEngines() const;
    Q_REQUIRED_RESULT virtual QStringList availableVoices() const;
    virtual void setEngine(const QString &engineName);
    virtual void testEngine(const EngineSettings &engineSettings);
};
}
