/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "soundmanager.h"

#include <QAudioOutput>
#include <QMediaPlayer>

SoundManager::SoundManager(QObject *parent)
    : QObject{parent}
    , mPlayer(new QMediaPlayer(this))
    , mAudioOutput(new QAudioOutput(this))
{
}

SoundManager::~SoundManager() = default;

void SoundManager::playSound(const QUrl &url)
{
    mPlayer->setAudioOutput(mAudioOutput);
    mPlayer->setSource(url);
    mAudioOutput->setVolume(50);
    mPlayer->play();
}
