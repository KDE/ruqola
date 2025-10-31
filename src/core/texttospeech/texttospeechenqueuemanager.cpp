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

TextToSpeechEnqueueInfo TextToSpeechEnqueueManager::value(qsizetype index) const
{
    return mEnqueueList.value(index);
}

bool TextToSpeechEnqueueManager::contains(const TextToSpeechEnqueueInfo &info) const
{
    // we can have several invalid element. Necessary if we don't have info.
    if (!info.isValid()) {
        return false;
    }
    return mEnqueueList.contains(info);
}

void TextToSpeechEnqueueManager::insertDummyInfo()
{
    TextToSpeechEnqueueInfo info;
    mEnqueueList.append(info);
}

QList<TextToSpeechEnqueueInfo> TextToSpeechEnqueueManager::enqueueList() const
{
    return mEnqueueList;
}

void TextToSpeechEnqueueManager::insert(const TextToSpeechEnqueueInfo &info)
{
    mEnqueueList.append(info);
}

#include "moc_texttospeechenqueuemanager.cpp"
