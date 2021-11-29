/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsoundwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QToolButton>

PlaySoundWidget::PlaySoundWidget(QWidget *parent)
    : QWidget(parent)
    , mMediaPlayer(new QMediaPlayer(this))
    , mPlayButton(new QPushButton(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mMediaPlayer->setObjectName(QStringLiteral("mMediaPlayer"));
    // connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    // mMediaPlayer->setVolume(50);

    // Allow to change volume

    connect(mMediaPlayer, &QMediaPlayer::stateChanged, this, &PlaySoundWidget::mediaStateChanged);

    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    mainLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &PlaySoundWidget::play);

    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(QStringLiteral("mSoundButton"));
    mSoundButton->setIcon(QIcon::fromTheme(QStringLiteral("player-volume")));
    connect(mSoundButton, &QToolButton::clicked, mMediaPlayer, &QMediaPlayer::setMuted);
    connect(mMediaPlayer, &QMediaPlayer::mutedChanged, this, &PlaySoundWidget::muteChanged);
    mainLayout->addWidget(mSoundButton);
    mSoundSlider->setObjectName(QStringLiteral("mSoundSlider"));
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(100);
    mSoundSlider->setTickPosition(QSlider::TicksAbove);
    connect(mSoundSlider, &QAbstractSlider::valueChanged, mMediaPlayer, &QMediaPlayer::setVolume);

    mainLayout->addWidget(mSoundSlider);
}

PlaySoundWidget::~PlaySoundWidget() = default;

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
