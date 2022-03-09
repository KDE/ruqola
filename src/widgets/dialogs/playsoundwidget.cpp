/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsoundwidget.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QTime>
#include <QToolButton>

PlaySoundWidget::PlaySoundWidget(QWidget *parent)
    : QWidget(parent)
    , mMediaPlayer(new QMediaPlayer(this))
    , mPlayButton(new QPushButton(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
    , mPositionSlider(new QSlider(Qt::Horizontal, this))
    , mLabelDuration(new QLabel(this))
    , mErrorLabel(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto playerLayout = new QHBoxLayout;
    playerLayout->setObjectName(QStringLiteral("playerLayout"));
    playerLayout->setContentsMargins({});
    mainLayout->addLayout(playerLayout);
    mainLayout->addWidget(mErrorLabel);

    mErrorLabel->setObjectName(QStringLiteral("mErrorLabel"));
    mLabelDuration->setObjectName(QStringLiteral("mLabelDuration"));

    mMediaPlayer->setObjectName(QStringLiteral("mMediaPlayer"));

    mPositionSlider->setObjectName(QStringLiteral("mPositionSlider"));
    mPositionSlider->setRange(0, 100);
    mPositionSlider->setValue(100);
    connect(mPositionSlider, &QAbstractSlider::sliderMoved, this, &PlaySoundWidget::setPosition);

    mSoundSlider->setObjectName(QStringLiteral("mSoundSlider"));
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(100);
    mSoundSlider->setTickPosition(QSlider::TicksAbove);

    connect(mSoundSlider, &QAbstractSlider::sliderMoved, mMediaPlayer, &QMediaPlayer::setVolume);

    connect(mMediaPlayer, &QMediaPlayer::positionChanged, this, &PlaySoundWidget::positionChanged);
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, this, &PlaySoundWidget::durationChanged);

    // Allow to change volume
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mMediaPlayer, &QMediaPlayer::stateChanged, this, &PlaySoundWidget::mediaStateChanged);
#endif
    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playerLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &PlaySoundWidget::play);

    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(QStringLiteral("mSoundButton"));
    mSoundButton->setIcon(QIcon::fromTheme(QStringLiteral("player-volume")));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mSoundButton, &QToolButton::clicked, mMediaPlayer, &QMediaPlayer::setMuted);
    connect(mMediaPlayer, &QMediaPlayer::mutedChanged, this, &PlaySoundWidget::muteChanged);
#endif
    playerLayout->addWidget(mPositionSlider);

    playerLayout->addWidget(mSoundButton);

    playerLayout->addWidget(mLabelDuration);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mSoundSlider, &QAbstractSlider::valueChanged, mMediaPlayer, &QMediaPlayer::setVolume);
    connect(mMediaPlayer, qOverload<QMediaPlayer::Error>(&QMediaPlayer::error), this, &PlaySoundWidget::handleError);
#endif
    playerLayout->addWidget(mSoundSlider);
}

PlaySoundWidget::~PlaySoundWidget() = default;

void PlaySoundWidget::positionChanged(qint64 progress)
{
    if (!mPositionSlider->isSliderDown())
        mPositionSlider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void PlaySoundWidget::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || mDuration) {
        const QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60, currentInfo % 60, (currentInfo * 1000) % 1000);
        const QTime totalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60, (mDuration * 1000) % 1000);
        QString format = QStringLiteral("mm:ss");
        if (mDuration > 3600)
            format = QStringLiteral("hh:mm:ss");
        tStr = currentTime.toString(format) + QStringLiteral(" / ") + totalTime.toString(format);
    }
    mLabelDuration->setText(tStr);
}

void PlaySoundWidget::durationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    mPositionSlider->setMaximum(mDuration);
}

void PlaySoundWidget::setPosition(int position)
{
    mMediaPlayer->setPosition(position * 1000);
}

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

void PlaySoundWidget::handleError()
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
