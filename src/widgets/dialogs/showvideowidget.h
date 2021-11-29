/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QMediaPlayer>
#include <QWidget>
class QPushButton;
class QSlider;
class QLabel;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowVideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowVideoWidget(QWidget *parent = nullptr);
    ~ShowVideoWidget() override;
    void setVideoUrl(const QUrl &videoPath);

private:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void muteChanged(bool state);
    void setPosition(int position);
    void handleError();
    void play();

    QMediaPlayer *const mMediaPlayer;
    QPushButton *const mPlayButton;
    QSlider *const mPositionSlider;
    QLabel *const mErrorLabel;
    QToolButton *const mSoundButton;
    QSlider *const mSoundSlider;
};

