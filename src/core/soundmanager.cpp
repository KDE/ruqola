/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "soundmanager.h"
using namespace Qt::Literals::StringLiterals;

#include <QAudioOutput>
#include <QMediaPlayer>

SoundManager::SoundManager(QObject *parent)
    : QObject{parent}
    , mPlayer(new QMediaPlayer(this))
    , mAudioOutput(new QAudioOutput(this))
{
    // Initialize
    constexpr float volume = 50.0 / 100.0;
    mAudioOutput->setVolume(volume);
}

SoundManager::~SoundManager() = default;

void SoundManager::playSound(const QUrl &url)
{
    mPlayer->setAudioOutput(mAudioOutput);
    mPlayer->setSource(url);
    mPlayer->play();
}

void SoundManager::setVolume(int volume)
{
    mAudioOutput->setVolume(static_cast<double>(volume) / 100.0);
}

#include "moc_soundmanager.cpp"
