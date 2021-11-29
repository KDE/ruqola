/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QMediaPlayer>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QPushButton;
class QToolButton;
class QSlider;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PlaySoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaySoundWidget(QWidget *parent = nullptr);
    ~PlaySoundWidget() override;
    void setAudioUrl(const QUrl &url);

private:
    void play();
    void muteChanged(bool state);
    void mediaStateChanged(QMediaPlayer::State state);
    QMediaPlayer *const mMediaPlayer;
    QPushButton *const mPlayButton;
    QToolButton *const mSoundButton;
    QSlider *const mSoundSlider;
};

