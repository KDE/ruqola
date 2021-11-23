/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "showvideowidget.h"
#include <KLocalizedString>

#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVideoWidget>

ShowVideoWidget::ShowVideoWidget(QWidget *parent)
    : QWidget(parent)
    , mMediaPlayer(new QMediaPlayer(this, QMediaPlayer::VideoSurface))
    , mPlayButton(new QPushButton(this))
    , mPositionSlider(new QSlider(Qt::Horizontal, this))
    , mErrorLabel(new QLabel(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mMediaPlayer->setObjectName(QStringLiteral("mMediaPlayer"));
    auto videoWidget = new QVideoWidget(this);
    videoWidget->setObjectName(QStringLiteral("videoWidget"));

    mainLayout->addWidget(videoWidget);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setObjectName(QStringLiteral("controlLayout"));
    controlLayout->setContentsMargins({});
    mainLayout->addLayout(controlLayout);

    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    controlLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &ShowVideoWidget::play);

    mPositionSlider->setObjectName(QStringLiteral("mPositionSlider"));
    mPositionSlider->setRange(0, 0);
    controlLayout->addWidget(mPositionSlider);

    connect(mPositionSlider, &QAbstractSlider::sliderMoved, this, &ShowVideoWidget::setPosition);

    mErrorLabel->setObjectName(QStringLiteral("mErrorLabel"));
    mErrorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mainLayout->addWidget(mErrorLabel);
    mMediaPlayer->setVideoOutput(videoWidget);
    connect(mMediaPlayer, &QMediaPlayer::stateChanged, this, &ShowVideoWidget::mediaStateChanged);
    connect(mMediaPlayer, &QMediaPlayer::positionChanged, this, &ShowVideoWidget::positionChanged);
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, this, &ShowVideoWidget::durationChanged);
    connect(mMediaPlayer, qOverload<QMediaPlayer::Error>(&QMediaPlayer::error), this, &ShowVideoWidget::handleError);

    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(QStringLiteral("mSoundButton"));
    mSoundButton->setIcon(QIcon::fromTheme(QStringLiteral("player-volume")));
    connect(mSoundButton, &QToolButton::clicked, mMediaPlayer, &QMediaPlayer::setMuted);
    connect(mMediaPlayer, &QMediaPlayer::mutedChanged, this, &ShowVideoWidget::muteChanged);
    controlLayout->addWidget(mSoundButton);
    mSoundSlider->setObjectName(QStringLiteral("mSoundSlider"));
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(100);
    mSoundSlider->setTickPosition(QSlider::TicksAbove);
    connect(mSoundSlider, &QAbstractSlider::valueChanged, mMediaPlayer, &QMediaPlayer::setVolume);

    controlLayout->addWidget(mSoundSlider);
}

ShowVideoWidget::~ShowVideoWidget() = default;

void ShowVideoWidget::muteChanged(bool state)
{
    mSoundButton->setIcon(state ? QIcon::fromTheme(QStringLiteral("player-volume-muted")) : QIcon::fromTheme(QStringLiteral("player-volume")));
}

void ShowVideoWidget::setVideoUrl(const QUrl &url)
{
    mErrorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mMediaPlayer->setMedia(url);
    mPlayButton->setEnabled(true);
}

void ShowVideoWidget::play()
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

void ShowVideoWidget::mediaStateChanged(QMediaPlayer::State state)
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

void ShowVideoWidget::positionChanged(qint64 position)
{
    mPositionSlider->setValue(position);
}

void ShowVideoWidget::durationChanged(qint64 duration)
{
    mPositionSlider->setRange(0, duration);
}

void ShowVideoWidget::setPosition(int position)
{
    mMediaPlayer->setPosition(position);
}

void ShowVideoWidget::handleError()
{
    mPlayButton->setEnabled(false);
    const QString errorString = mMediaPlayer->errorString();
    QString message = i18n("Error: "); // i18n ?
    if (errorString.isEmpty()) {
        message += QStringLiteral(" #") + QString::number(int(mMediaPlayer->error()));
    } else {
        message += errorString;
    }
    mErrorLabel->setText(message);
}
