/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideowidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"
#include "ruqolaglobalconfig.h"

#include <KLocalizedString>

#include <KMessageWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QTime>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVideoWidget>

#include <QAudioDevice>
#include <QAudioOutput>
#include <QComboBox>
#include <QMediaDevices>

ShowVideoWidget::ShowVideoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mMediaPlayer(new QMediaPlayer(this))
    , mPlayButton(new QPushButton(this))
    , mPositionSlider(new QSlider(Qt::Horizontal, this))
    , mMessageWidget(new KMessageWidget(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
    , mLabelDuration(new QLabel(this))
    , mLabelPercentSound(new QLabel(this))
    , mAudioOutput(new QAudioOutput(this))
    , mSoundDeviceComboBox(new QComboBox(this))
    , mRocketChatAccount(account)
{
    mMediaPlayer->setAudioOutput(mAudioOutput);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mSoundDeviceComboBox->setObjectName(u"mSoundDeviceComboBox"_s);
    mainLayout->addWidget(mSoundDeviceComboBox);
    // TODO initialize it.

    mLabelDuration->setObjectName(u"mLabelDuration"_s);

    mMediaPlayer->setObjectName(u"mMediaPlayer"_s);
    auto videoWidget = new QVideoWidget(this);
    videoWidget->setObjectName(u"videoWidget"_s);

    mainLayout->addWidget(videoWidget, 1);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setObjectName(u"controlLayout"_s);
    controlLayout->setContentsMargins({});
    mainLayout->addLayout(controlLayout);

    mPlayButton->setObjectName(u"mPlayButton"_s);
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    mLabelDuration->setTextFormat(Qt::PlainText);
    mLabelPercentSound->setTextFormat(Qt::PlainText);

    controlLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &ShowVideoWidget::play);

    mPositionSlider->setObjectName(u"mPositionSlider"_s);
    mPositionSlider->setRange(0, 0);
    controlLayout->addWidget(mPositionSlider);
    controlLayout->addWidget(mLabelDuration);

    connect(mPositionSlider, &QAbstractSlider::sliderMoved, this, &ShowVideoWidget::setPosition);
    connect(mPositionSlider, &QAbstractSlider::valueChanged, this, &ShowVideoWidget::setPosition);

    mMessageWidget->setObjectName(u"mMessageWidget"_s);
    mainLayout->addWidget(mMessageWidget);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Error);
    mMessageWidget->setWordWrap(true);

    mMediaPlayer->setVideoOutput(videoWidget);
    connect(mMediaPlayer, &QMediaPlayer::playbackStateChanged, this, &ShowVideoWidget::mediaStateChanged);
    connect(mMediaPlayer, &QMediaPlayer::positionChanged, this, &ShowVideoWidget::slotPositionChanged);
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, this, &ShowVideoWidget::slotDurationChanged);
    connect(mMediaPlayer, &QMediaPlayer::errorChanged, this, &ShowVideoWidget::handleError);
    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(u"mSoundButton"_s);
    mSoundButton->setIcon(QIcon::fromTheme(u"player-volume"_s));
    connect(mSoundButton, &QToolButton::clicked, mAudioOutput, &QAudioOutput::setMuted);
    connect(mAudioOutput, &QAudioOutput::mutedChanged, this, &ShowVideoWidget::slotMuteChanged);
    controlLayout->addWidget(mSoundButton);
    mSoundSlider->setObjectName(u"mSoundSlider"_s);
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(RuqolaGlobalConfig::self()->soundVolume());
    mSoundSlider->setTickPosition(QSlider::TicksAbove);
    connect(mSoundSlider, &QAbstractSlider::sliderMoved, this, &ShowVideoWidget::slotVolumeChanged);
    controlLayout->addWidget(mSoundSlider);
    controlLayout->addWidget(mLabelPercentSound);
    QFontMetrics f(font());
    mLabelPercentSound->setFixedWidth(f.horizontalAdvance(u"MMM"_s));
    slotVolumeChanged(mSoundSlider->value());
    initializeAudioOutput();
}

ShowVideoWidget::~ShowVideoWidget()
{
    RuqolaGlobalConfig::self()->setSoundVolume(mSoundSlider->value());
    RuqolaGlobalConfig::self()->save();
}

void ShowVideoWidget::initializeAudioOutput()
{
    mSoundDeviceComboBox->addItem(i18n("Default"), QVariant::fromValue(QAudioDevice()));
    for (const auto &deviceInfo : QMediaDevices::audioOutputs()) {
        mSoundDeviceComboBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }
    connect(mSoundDeviceComboBox, &QComboBox::activated, this, &ShowVideoWidget::audioOutputChanged);
}

void ShowVideoWidget::audioOutputChanged(int index)
{
    const auto device = mSoundDeviceComboBox->itemData(index).value<QAudioDevice>();
    mMediaPlayer->audioOutput()->setDevice(device);
}

void ShowVideoWidget::slotPositionChanged(qint64 progress)
{
    const qint64 newValue = progress / 1000;
    if (!mPositionSlider->isSliderDown()) {
        mPositionSlider->setValue(newValue);
    }

    updateDurationInfo(newValue);
}

void ShowVideoWidget::slotVolumeChanged(int position)
{
    mAudioOutput->setVolume(position / 100.0);
    mLabelPercentSound->setText(u"%1%"_s.arg(position));
}

void ShowVideoWidget::updateDurationInfo(qint64 currentInfo)
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

void ShowVideoWidget::slotMuteChanged(bool state)
{
    mSoundButton->setIcon(state ? QIcon::fromTheme(u"player-volume-muted"_s) : QIcon::fromTheme(u"player-volume"_s));
}

void ShowVideoWidget::slotAttachmentFileDownloadDone(const QString &url)
{
    const QUrl localUrl = QUrl::fromLocalFile(url);
    Q_EMIT updateTitle(localUrl);
    mMessageWidget->setText(QString());
    mMessageWidget->hide();
    setWindowFilePath(url);
    mMediaPlayer->setSource(localUrl);
    mPlayButton->setEnabled(true);
}

void ShowVideoWidget::setVideoPath(const QString &url, bool isLocalFile)
{
    if (mRocketChatAccount && !isLocalFile) {
        MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
        info.attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Video;
        info.actionType = MessageAttachmentDownloadAndSaveJob::ActionType::DownloadOnly;
        info.needToDownloadAttachment = !mRocketChatAccount->attachmentIsInLocalCache(url);
        info.parentWidget = this;
        info.attachmentPath = url;
        auto job = new MessageAttachmentDownloadAndSaveJob(this);
        connect(job, &MessageAttachmentDownloadAndSaveJob::attachmentFileDownloadDone, this, &ShowVideoWidget::slotAttachmentFileDownloadDone);
        job->setRocketChatAccount(mRocketChatAccount);
        job->setInfo(info);
        job->start();
    } else {
        slotAttachmentFileDownloadDone(url);
    }
}

QUrl ShowVideoWidget::videoUrl() const
{
    return mMediaPlayer->source();
}

void ShowVideoWidget::play()
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

void ShowVideoWidget::mediaStateChanged(QMediaPlayer::PlaybackState state)
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

void ShowVideoWidget::slotDurationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    mPositionSlider->setMaximum(mDuration);
}

void ShowVideoWidget::setPosition(int position)
{
    mMediaPlayer->setPosition(position * 1000);
}

void ShowVideoWidget::handleError()
{
    mPlayButton->setEnabled(false);
    const QString errorString = mMediaPlayer->errorString();
    QString message = i18n("Error: ");
    if (errorString.isEmpty()) {
        message += u" #"_s + QString::number(int(mMediaPlayer->error()));
    } else {
        message += errorString;
    }
    mMessageWidget->setText(message);
    mMessageWidget->animatedShow();
}

#include "moc_showvideowidget.cpp"
