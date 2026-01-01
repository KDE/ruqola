/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsoundwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"
#include "ruqolaglobalconfig.h"

#include <KLocalizedString>
#include <KMessageWidget>

#include <QAudioDevice>
#include <QAudioOutput>
#include <QComboBox>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QTime>
#include <QToolButton>

PlaySoundWidget::PlaySoundWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mMediaPlayer(new QMediaPlayer(this))
    , mPlayButton(new QPushButton(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
    , mPositionSlider(new QSlider(Qt::Horizontal, this))
    , mLabelDuration(new QLabel(this))
    , mMessageWidget(new KMessageWidget(this))
    , mLabelPercentSound(new QLabel(this))
    , mAudioOutput(new QAudioOutput(this))
    , mDeviceComboBox(new QComboBox(this))
    , mRocketChatAccount(account)
{
    mMediaPlayer->setAudioOutput(mAudioOutput);
    mDeviceComboBox->setObjectName(u"mDeviceComboBox"_s);
    initializeAudioOutput();

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mDeviceComboBox);

    auto playerLayout = new QHBoxLayout;
    playerLayout->setObjectName(u"playerLayout"_s);
    playerLayout->setContentsMargins({});
    mainLayout->addLayout(playerLayout);
    mMessageWidget->setObjectName(u"mMessageWidget"_s);
    mainLayout->addWidget(mMessageWidget);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Error);
    mMessageWidget->setWordWrap(true);
    mLabelDuration->setObjectName(u"mLabelDuration"_s);
    mLabelPercentSound->setObjectName(u"mLabelPercentSound"_s);

    mLabelDuration->setTextFormat(Qt::PlainText);
    mLabelPercentSound->setTextFormat(Qt::PlainText);

    mMediaPlayer->setObjectName(u"mMediaPlayer"_s);

    mPositionSlider->setObjectName(u"mPositionSlider"_s);
    mPositionSlider->setRange(0, 100);
    mPositionSlider->setValue(100);
    connect(mPositionSlider, &QAbstractSlider::sliderMoved, this, &PlaySoundWidget::setPosition);

    mSoundSlider->setObjectName(u"mSoundSlider"_s);
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(RuqolaGlobalConfig::self()->soundVolume());
    mSoundSlider->setTickPosition(QSlider::TicksAbove);

    connect(mSoundSlider, &QAbstractSlider::sliderMoved, this, &PlaySoundWidget::slotVolumeChanged);

    connect(mMediaPlayer, &QMediaPlayer::positionChanged, this, &PlaySoundWidget::slotPositionChanged);
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, this, &PlaySoundWidget::slotDurationChanged);

    // Allow to change volume

    connect(mMediaPlayer, &QMediaPlayer::playbackStateChanged, this, &PlaySoundWidget::mediaStateChanged);
    mPlayButton->setObjectName(u"mPlayButton"_s);
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playerLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &PlaySoundWidget::play);

    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(u"mSoundButton"_s);
    mSoundButton->setIcon(QIcon::fromTheme(u"player-volume"_s));
    connect(mSoundButton, &QToolButton::clicked, mAudioOutput, &QAudioOutput::setMuted);
    connect(mAudioOutput, &QAudioOutput::mutedChanged, this, &PlaySoundWidget::muteChanged);
    playerLayout->addWidget(mPositionSlider);

    playerLayout->addWidget(mSoundButton);

    playerLayout->addWidget(mLabelDuration);

    connect(mMediaPlayer, &QMediaPlayer::errorChanged, this, &PlaySoundWidget::handleError);

    playerLayout->addWidget(mSoundSlider);
    playerLayout->addWidget(mLabelPercentSound);

    QFontMetrics f(font());
    mLabelPercentSound->setFixedWidth(f.horizontalAdvance(u"MMM"_s));
    slotVolumeChanged(mSoundSlider->value());
}

PlaySoundWidget::~PlaySoundWidget()
{
    RuqolaGlobalConfig::self()->setSoundVolume(mSoundSlider->value());
    RuqolaGlobalConfig::self()->save();
}

void PlaySoundWidget::initializeAudioOutput()
{
    mDeviceComboBox->addItem(i18n("Default"), QVariant::fromValue(QAudioDevice()));
    for (const auto &deviceInfo : QMediaDevices::audioOutputs()) {
        mDeviceComboBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }
    connect(mDeviceComboBox, &QComboBox::activated, this, &PlaySoundWidget::audioOutputChanged);
}

void PlaySoundWidget::audioOutputChanged(int index)
{
    const auto device = mDeviceComboBox->itemData(index).value<QAudioDevice>();
    mMediaPlayer->audioOutput()->setDevice(device);
}

void PlaySoundWidget::slotPositionChanged(qint64 progress)
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
        QString format = u"mm:ss"_s;
        if (mDuration > 3600)
            format = u"hh:mm:ss"_s;
        tStr = currentTime.toString(format) + u" / "_s + totalTime.toString(format);
    }
    mLabelDuration->setText(tStr);
}

void PlaySoundWidget::slotVolumeChanged(int position)
{
    mAudioOutput->setVolume(position / 100.0);
    mLabelPercentSound->setText(u"%1%"_s.arg(position));
}

void PlaySoundWidget::slotDurationChanged(qint64 duration)
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
    mSoundButton->setIcon(state ? QIcon::fromTheme(u"player-volume-muted"_s) : QIcon::fromTheme(u"player-volume"_s));
}

QUrl PlaySoundWidget::audioUrl() const
{
    return mMediaPlayer->source();
}

void PlaySoundWidget::slotAttachmentFileDownloadDone(const QString &url)
{
    const QUrl localUrl = QUrl::fromLocalFile(url);
    Q_EMIT updateTitle(localUrl);
    // setWindowFilePath(localUrl);
    mMediaPlayer->setSource(localUrl);
    mPlayButton->setEnabled(true);
}

void PlaySoundWidget::setLocalAudioPath(const QString &url)
{
    slotAttachmentFileDownloadDone(url);
}

void PlaySoundWidget::setAudioPath(const QString &url)
{
    if (mRocketChatAccount) {
        MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
        info.attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Sound;
        info.actionType = MessageAttachmentDownloadAndSaveJob::ActionType::DownloadOnly;
        info.needToDownloadAttachment = !mRocketChatAccount->attachmentIsInLocalCache(url);
        info.parentWidget = this;
        info.attachmentPath = url;
        auto job = new MessageAttachmentDownloadAndSaveJob(this);
        connect(job, &MessageAttachmentDownloadAndSaveJob::attachmentFileDownloadDone, this, &PlaySoundWidget::slotAttachmentFileDownloadDone);
        job->setRocketChatAccount(mRocketChatAccount);
        job->setInfo(info);
        job->start();
    } else {
        slotAttachmentFileDownloadDone(url);
    }
}

void PlaySoundWidget::play()
{
    switch (mMediaPlayer->playbackState()) {
    case QMediaPlayer::PlayingState:
        mMediaPlayer->pause();
        break;
    default:
        mMediaPlayer->play();
        break;
    }
}

void PlaySoundWidget::mediaStateChanged(QMediaPlayer::PlaybackState state)
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

void PlaySoundWidget::handleError()
{
    mPlayButton->setEnabled(false);
    const QString errorString = mMediaPlayer->errorString();
    QString message = i18n("Error: "); // i18n ?
    if (errorString.isEmpty()) {
        message += u" #"_s + QString::number(int(mMediaPlayer->error()));
    } else {
        message += errorString;
    }
    mMessageWidget->setText(message);
    mMessageWidget->animatedShow();
}

#include "moc_playsoundwidget.cpp"
