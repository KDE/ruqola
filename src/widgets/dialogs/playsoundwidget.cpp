/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "playsoundwidget.h"
#include <QHBoxLayout>
#include <QMediaPlayer>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QToolButton>

PlaySoundWidget::PlaySoundWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mMediaPlayer = new QMediaPlayer(this);
    mMediaPlayer->setObjectName(QStringLiteral("mMediaPlayer"));
    //connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    //mMediaPlayer->setVolume(50);

    //Allow to change volume

    connect(mMediaPlayer, &QMediaPlayer::stateChanged,
            this, &PlaySoundWidget::mediaStateChanged);

    mPlayButton = new QPushButton(this);
    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    mainLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked,
            this, &PlaySoundWidget::play);

    mSoundButton = new QToolButton(this);
    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(QStringLiteral("mSoundButton"));
    mSoundButton->setIcon(QIcon::fromTheme(QStringLiteral("player-volume")));
    connect(mSoundButton, &QToolButton::clicked, mMediaPlayer, &QMediaPlayer::setMuted);
    connect(mMediaPlayer, &QMediaPlayer::mutedChanged, this, &PlaySoundWidget::muteChanged);
    mainLayout->addWidget(mSoundButton);
    mSoundSlider = new QSlider(Qt::Horizontal, this);
    mSoundSlider->setObjectName(QStringLiteral("mSoundSlider"));
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(100);
    mSoundSlider->setTickPosition(QSlider::TicksAbove);
    connect(mSoundSlider, &QAbstractSlider::valueChanged,
            mMediaPlayer, &QMediaPlayer::setVolume);

    mainLayout->addWidget(mSoundSlider);
}

PlaySoundWidget::~PlaySoundWidget()
{
}

void PlaySoundWidget::muteChanged(bool state)
{
    mSoundButton->setIcon(state ? QIcon::fromTheme(QStringLiteral("player-volume-muted")) : QIcon::fromTheme(QStringLiteral("player-volume")));
}

void PlaySoundWidget::setAudioUrl(const QUrl &url)
{
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mMediaPlayer->setMedia(url);
    mPlayButton->setEnabled(true);
}

void PlaySoundWidget::play()
{
    switch (mMediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        mMediaPlayer->pause();
        break;
    default:
        mMediaPlayer->play();
        break;
    }
}

void PlaySoundWidget::mediaStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}
