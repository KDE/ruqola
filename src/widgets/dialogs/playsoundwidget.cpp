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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mMediaPlayer, &QMediaPlayer::stateChanged, this, &PlaySoundWidget::mediaStateChanged);
#endif
    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    mainLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &PlaySoundWidget::play);

    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(QStringLiteral("mSoundButton"));
    mSoundButton->setIcon(QIcon::fromTheme(QStringLiteral("player-volume")));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mSoundButton, &QToolButton::clicked, mMediaPlayer, &QMediaPlayer::setMuted);
    connect(mMediaPlayer, &QMediaPlayer::mutedChanged, this, &PlaySoundWidget::muteChanged);
#endif
    mainLayout->addWidget(mSoundButton);
    mSoundSlider->setObjectName(QStringLiteral("mSoundSlider"));
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(100);
    mSoundSlider->setTickPosition(QSlider::TicksAbove);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mSoundSlider, &QAbstractSlider::valueChanged, mMediaPlayer, &QMediaPlayer::setVolume);
#endif
    mainLayout->addWidget(mSoundSlider);
}

PlaySoundWidget::~PlaySoundWidget() = default;

void PlaySoundWidget::muteChanged(bool state)
{
    mSoundButton->setIcon(state ? QIcon::fromTheme(QStringLiteral("player-volume-muted")) : QIcon::fromTheme(QStringLiteral("player-volume")));
}

void PlaySoundWidget::setAudioUrl(const QUrl &url)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mMediaPlayer->setMedia(url);
    mPlayButton->setEnabled(true);
#endif
}

void PlaySoundWidget::play()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    switch (mMediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        mMediaPlayer->pause();
        break;
    default:
        mMediaPlayer->play();
        break;
    }
#endif
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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
#endif
