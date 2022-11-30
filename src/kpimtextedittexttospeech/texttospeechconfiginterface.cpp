/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfiginterface.h"
#include <KLocalizedString>
#include <QDebug>
#include <QTextToSpeech>
using namespace KPIMTextEditTextToSpeech;

TextToSpeechConfigInterface::TextToSpeechConfigInterface(QObject *parent)
    : AbstractTextToSpeechConfigInterface(parent)
{
}

TextToSpeechConfigInterface::~TextToSpeechConfigInterface() = default;

QStringList TextToSpeechConfigInterface::availableVoices() const
{
    QStringList lst;
    const QVector<QVoice> voices = mTextToSpeech->availableVoices();
    lst.reserve(voices.count());
    for (const QVoice &voice : voices) {
        lst << voice.name();
    }
    return lst;
}

QStringList TextToSpeechConfigInterface::availableEngines() const
{
    return mTextToSpeech->availableEngines();
}

QVector<QLocale> TextToSpeechConfigInterface::availableLocales() const
{
    return mTextToSpeech->availableLocales();
}

QLocale TextToSpeechConfigInterface::locale() const
{
    return mTextToSpeech->locale();
}

void TextToSpeechConfigInterface::setEngine(const QString &engineName)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    if (mTextToSpeech && (mTextToSpeech->engine() != engineName))
#endif
    {
        delete mTextToSpeech;
        mTextToSpeech = new QTextToSpeech(engineName, this);
    }
}

void TextToSpeechConfigInterface::testEngine(const EngineSettings &engineSettings)
{
    const int rate = engineSettings.rate;
    const double rateDouble = rate / 100.0;
    mTextToSpeech->setRate(rateDouble);
    const int pitch = engineSettings.pitch;
    const double pitchDouble = pitch / 100.0;
    mTextToSpeech->setPitch(pitchDouble);
    const int volume = engineSettings.volume;
    const double volumeDouble = volume / 100.0;
    mTextToSpeech->setVolume(volumeDouble);
    mTextToSpeech->setLocale(QLocale(engineSettings.localeName));

    // TODO change text ?
    mTextToSpeech->say(i18n("Morning, this is the test for testing settings."));
}
