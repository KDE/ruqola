/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechmanager.h"

#include <QTextToSpeech>

TextToSpeechManager::TextToSpeechManager(QObject *parent)
    : QObject{parent}
{
}

TextToSpeechManager::~TextToSpeechManager() = default;

TextToSpeechManager *TextToSpeechManager::self()
{
    static TextToSpeechManager s_self;
    return &s_self;
}

QTextToSpeech *TextToSpeechManager::textToSpeech()
{
    if (!mTextToSpeech) {
        mTextToSpeech = new QTextToSpeech(this);
    }
    return mTextToSpeech;
}
