/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "abstracttexttospeechinterface.h"
using namespace TextEditTextToSpeech;
AbstractTextToSpeechInterface::AbstractTextToSpeechInterface(QObject *parent)
    : QObject(parent)
{
}

AbstractTextToSpeechInterface::~AbstractTextToSpeechInterface() = default;

bool AbstractTextToSpeechInterface::isReady() const
{
    return true;
}

void AbstractTextToSpeechInterface::say(const QString &text)
{
    Q_UNUSED(text)
}

double AbstractTextToSpeechInterface::volume() const
{
    // Value between 0.0 and 1.0
    // => 0.5 => 50% volume.
    return 0.5;
}

void AbstractTextToSpeechInterface::setVolume(double value)
{
    Q_UNUSED(value)
}

void AbstractTextToSpeechInterface::reloadSettings()
{
}
