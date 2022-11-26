/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedittexttospeech_export.h"
#include <QObject>
#include <memory>

class QTextToSpeech;

namespace KPIMTextEditTextToSpeech
{
class TextToSpeechPrivate;
/**
 * @brief The TextToSpeech class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT TextToSpeech : public QObject
{
    Q_OBJECT
public:
    static TextToSpeech *self();

    ~TextToSpeech() override;

    Q_REQUIRED_RESULT bool isReady() const;

    enum State {
        Ready = 0,
        Speaking,
        Paused,
        BackendError,
    };

    Q_REQUIRED_RESULT double volume() const;
    Q_REQUIRED_RESULT QVector<QLocale> availableLocales() const;
    Q_REQUIRED_RESULT QStringList availableEngines() const;
    Q_REQUIRED_RESULT QStringList availableVoices() const;

    Q_REQUIRED_RESULT QLocale locale() const;

    void reloadSettings();
public Q_SLOTS:
    void say(const QString &text);
    void stop();
    void pause();
    void resume();

    void setRate(double rate);
    void setPitch(double pitch);
    void setVolume(double volume);
    void setLocale(const QLocale &locale) const;

Q_SIGNALS:
    void stateChanged(TextToSpeech::State);

private:
    void slotStateChanged();
    explicit TextToSpeech(QObject *parent = nullptr);

    std::unique_ptr<TextToSpeechPrivate> const d;
};
}
