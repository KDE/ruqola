/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "texttospeechenqueuemanager.h"

TextToSpeechEnqueueManager::TextToSpeechEnqueueManager(QObject *parent)
    : QObject{parent}
{
}

TextToSpeechEnqueueManager::~TextToSpeechEnqueueManager() = default;

void TextToSpeechEnqueueManager::clear()
{
    mEnqueueList.clear();
}

TextToSpeechEnqueueInfo TextToSpeechEnqueueManager::textToSpeechInfo(qsizetype index)
{
    const TextToSpeechEnqueueInfo info = mEnqueueList.take(index);
    return info;
}

bool TextToSpeechEnqueueManager::contains(qsizetype index) const
{
    return mEnqueueList.contains(index);
}

void TextToSpeechEnqueueManager::insert(qsizetype index, const TextToSpeechEnqueueInfo &info)
{
    if (!info.isValid()) {
        return;
    }
    for (const auto &[key, value] : mEnqueueList.asKeyValueRange()) {
        if (value == info) {
            return;
        }
    }
    mEnqueueList.insert(index, info);
}

#include "moc_texttospeechenqueuemanager.cpp"
