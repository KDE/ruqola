/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "abstracttexttospeechconfiginterface.h"

using namespace TextEditTextToSpeech;

AbstractTextToSpeechConfigInterface::AbstractTextToSpeechConfigInterface(QObject *parent)
    : QObject(parent)
{
}

AbstractTextToSpeechConfigInterface::~AbstractTextToSpeechConfigInterface() = default;

QVector<QLocale> AbstractTextToSpeechConfigInterface::availableLocales() const
{
    return {};
}

QLocale AbstractTextToSpeechConfigInterface::locale() const
{
    return {};
}

void AbstractTextToSpeechConfigInterface::setLocale(const QLocale &locale)
{
    Q_UNUSED(locale)
}

QStringList AbstractTextToSpeechConfigInterface::availableEngines() const
{
    return {};
}

QStringList AbstractTextToSpeechConfigInterface::availableVoices() const
{
    return {};
}

void AbstractTextToSpeechConfigInterface::setEngine(const QString &engineName)
{
    Q_UNUSED(engineName)
}

void AbstractTextToSpeechConfigInterface::testEngine(const EngineSettings &engineSettings)
{
    Q_UNUSED(engineSettings)
}
