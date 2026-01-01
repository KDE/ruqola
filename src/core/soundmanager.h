/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QAudioOutput;
class QMediaPlayer;
class SoundManager : public QObject
{
    Q_OBJECT
public:
    explicit SoundManager(QObject *parent = nullptr);
    ~SoundManager() override;

    void playSound(const QUrl &url);

    void setVolume(int volume);

private:
    QMediaPlayer *const mPlayer;
    QAudioOutput *const mAudioOutput;
};
