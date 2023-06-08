/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showsoundwidget.h"
#include "ruqolaglobalconfig.h"
#include <QAudioDevice>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QToolButton>

ShowSoundWidget::ShowSoundWidget(QWidget *parent)
    : QWidget{parent}
    , mPlayButton(new QPushButton(this))
    , mPositionSlider(new QSlider(Qt::Horizontal, this))
    , mErrorLabel(new QLabel(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
    , mLabelDuration(new QLabel(this))
    , mLabelPercentSound(new QLabel(this))
    , mDeviceComboBox(new QComboBox(this))
    , mDevices(new QMediaDevices(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mDeviceComboBox->setObjectName(QStringLiteral("mDeviceComboBox"));
    mainLayout->addWidget(mDeviceComboBox);

    mLabelDuration->setObjectName(QStringLiteral("mLabelDuration"));

    auto controlLayout = new QHBoxLayout;
    controlLayout->setObjectName(QStringLiteral("controlLayout"));
    controlLayout->setContentsMargins({});
    mainLayout->addLayout(controlLayout);

    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    mErrorLabel->setTextFormat(Qt::PlainText);
    mLabelDuration->setTextFormat(Qt::PlainText);
    mLabelPercentSound->setTextFormat(Qt::PlainText);

    controlLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &ShowSoundWidget::play);

    mPositionSlider->setObjectName(QStringLiteral("mPositionSlider"));
    mPositionSlider->setRange(0, 0);
    controlLayout->addWidget(mPositionSlider);
    controlLayout->addWidget(mLabelDuration);

    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(QStringLiteral("mSoundButton"));
    mSoundButton->setIcon(QIcon::fromTheme(QStringLiteral("player-volume")));

#if 0
    connect(mSoundButton, &QToolButton::clicked, mAudioOutput, &ShowSoundWidget::setMuted);
    connect(mAudioOutput, &QAudioOutput::mutedChanged, this, &ShowSoundWidget::slotMuteChanged);
#endif
    controlLayout->addWidget(mSoundButton);
    mSoundSlider->setObjectName(QStringLiteral("mSoundSlider"));
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(RuqolaGlobalConfig::self()->soundVolume());
    mSoundSlider->setTickPosition(QSlider::TicksAbove);
    connect(mSoundSlider, &QAbstractSlider::sliderMoved, this, &ShowSoundWidget::slotVolumeChanged);
    controlLayout->addWidget(mSoundSlider);
    controlLayout->addWidget(mLabelPercentSound);
    QFontMetrics f(font());
    mLabelPercentSound->setFixedWidth(f.horizontalAdvance(QStringLiteral("MMM")));
    slotVolumeChanged(mSoundSlider->value());
    connect(mDevices, &QMediaDevices::audioOutputsChanged, this, &ShowSoundWidget::updateAudioDevices);
    connect(mDeviceComboBox, &QComboBox::activated, this, &ShowSoundWidget::deviceChanged);

    initializeAudioComboBox();
}

ShowSoundWidget::~ShowSoundWidget()
{
}

void ShowSoundWidget::deviceChanged(int index)
{
#if 0
    m_generator->stop();
    m_audioOutput->stop();
    m_audioOutput->disconnect(this);
    initializeAudio(mDeviceComboBox->itemData(index).value<QAudioDevice>());
#endif
}

void ShowSoundWidget::updateAudioDevices()
{
    mDeviceComboBox->clear();
    const QList<QAudioDevice> devices = mDevices->audioOutputs();
    for (const QAudioDevice &deviceInfo : devices)
        mDeviceComboBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
}

void ShowSoundWidget::initializeAudioComboBox()
{
    const QAudioDevice &defaultDeviceInfo = mDevices->defaultAudioOutput();
    mDeviceComboBox->addItem(defaultDeviceInfo.description(), QVariant::fromValue(defaultDeviceInfo));
    for (auto &deviceInfo : mDevices->audioOutputs()) {
        if (deviceInfo != defaultDeviceInfo)
            mDeviceComboBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }
}

void ShowSoundWidget::slotMuteChanged(bool state)
{
    mSoundButton->setIcon(state ? QIcon::fromTheme(QStringLiteral("player-volume-muted")) : QIcon::fromTheme(QStringLiteral("player-volume")));
}

void ShowSoundWidget::slotVolumeChanged(int position)
{
    // TODO mAudioOutput->setVolume(position / 100.0);
    mLabelPercentSound->setText(QStringLiteral("%1%").arg(position));
}

void ShowSoundWidget::setSoundUrl(const QUrl &videoPath)
{
    // TODO
}

QUrl ShowSoundWidget::soundUrl() const
{
    // TODO
    return {};
}

void ShowSoundWidget::play()
{
}
