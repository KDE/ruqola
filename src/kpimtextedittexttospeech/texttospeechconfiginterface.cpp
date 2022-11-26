/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfiginterface.h"
#include <QTextToSpeech>
using namespace KPIMTextEditTextToSpeech;

TextToSpeechConfigInterface::TextToSpeechConfigInterface(QObject *parent)
    : AbstractTextToSpeechConfigInterface(parent)
    , mTextToSpeech(new QTextToSpeech(this))
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
    delete mTextToSpeech;
    mTextToSpeech = new QTextToSpeech(engineName, this);
}
