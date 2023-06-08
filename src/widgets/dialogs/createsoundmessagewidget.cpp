/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidget.h"
#include <KLocalizedString>
#include <QAudioInput>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

CreateSoundMessageWidget::CreateSoundMessageWidget(QWidget *parent)
    : QWidget{parent}
    , mRecordButton(new QToolButton(this))
    , mPauseButton(new QToolButton(this))
    , mStopButton(new QToolButton(this))
    , mLabelDuration(new QLabel(this))
    , mAudioRecorder(new QMediaRecorder(this))
    , mDeviceComboBox(new QComboBox(this))
{
    mCaptureSession.setRecorder(mAudioRecorder);
    mCaptureSession.setAudioInput(new QAudioInput(this));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mDeviceComboBox->setObjectName(QStringLiteral("mDeviceComboBox"));
    mainLayout->addWidget(mDeviceComboBox);

    auto soundWidgetLayout = new QHBoxLayout;
    soundWidgetLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addLayout(soundWidgetLayout);

    mStopButton->setObjectName(QStringLiteral("mStopButton"));
    mStopButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
    soundWidgetLayout->addWidget(mStopButton);
    connect(mStopButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::stop);

    mPauseButton->setObjectName(QStringLiteral("mPauseButton"));
    mPauseButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-pause")));
    soundWidgetLayout->addWidget(mPauseButton);
    connect(mPauseButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::pause);

    mRecordButton->setObjectName(QStringLiteral("mRecordButton"));
    mRecordButton->setIcon(QIcon::fromTheme(QStringLiteral("media-record")));
    soundWidgetLayout->addWidget(mRecordButton);
    connect(mRecordButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::record);

    mLabelDuration->setObjectName(QStringLiteral("mLabelDuration"));
    mLabelDuration->setTextFormat(Qt::PlainText);
    soundWidgetLayout->addWidget(mLabelDuration);

    connect(mAudioRecorder, &QMediaRecorder::durationChanged, this, &CreateSoundMessageWidget::updateRecordTime);
    connect(mAudioRecorder, &QMediaRecorder::recorderStateChanged, this, &CreateSoundMessageWidget::updateRecorderState);
    //    connect(mAudioRecorder, &QMediaRecorder::errorChanged, this,
    //            &CreateSoundMessageWidget::displayErrorMessage);
}

CreateSoundMessageWidget::~CreateSoundMessageWidget() = default;

void CreateSoundMessageWidget::updateRecordTime(qint64 duration)
{
    const QString str = i18n("Recorded %1 sec", duration / 1000);
    mLabelDuration->setText(str);
}

void CreateSoundMessageWidget::stop()
{
    mAudioRecorder->stop();
}

void CreateSoundMessageWidget::record()
{
    mAudioRecorder->record();
    Q_EMIT recordDone();
}

void CreateSoundMessageWidget::pause()
{
    mAudioRecorder->pause();
}

void CreateSoundMessageWidget::updateRecorderState(QMediaRecorder::RecorderState state)
{
    switch (state) {
    case QMediaRecorder::StoppedState:
        mRecordButton->setEnabled(true);
        mPauseButton->setEnabled(true);
        mStopButton->setEnabled(false);
        break;
    case QMediaRecorder::PausedState:
        mRecordButton->setEnabled(true);
        mPauseButton->setEnabled(false);
        mStopButton->setEnabled(true);
        break;
    case QMediaRecorder::RecordingState:
        mRecordButton->setEnabled(false);
        mPauseButton->setEnabled(true);
        mStopButton->setEnabled(true);
        break;
    }
}
