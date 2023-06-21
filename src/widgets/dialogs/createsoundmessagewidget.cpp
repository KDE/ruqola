/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidget.h"
#include <KLocalizedString>
#include <KMessageWidget>
#include <QAudioDevice>
#include <QAudioInput>
#include <QComboBox>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QTemporaryFile>
#include <QToolButton>

CreateSoundMessageWidget::CreateSoundMessageWidget(QWidget *parent)
    : QWidget{parent}
    , mRecordButton(new QToolButton(this))
    , mPauseButton(new QToolButton(this))
    , mStopButton(new QToolButton(this))
    , mLabelDuration(new QLabel(this))
    , mAudioRecorder(new QMediaRecorder(this))
    , mDeviceComboBox(new QComboBox(this))
    , mMessageWidget(new KMessageWidget(this))
{
    mCaptureSession.setRecorder(mAudioRecorder);
    mCaptureSession.setAudioInput(new QAudioInput(this));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mMessageWidget->setObjectName(QStringLiteral("mMessageWidget"));
    mainLayout->addWidget(mMessageWidget);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Information);
    mMessageWidget->setWordWrap(true);

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
    connect(mAudioRecorder, &QMediaRecorder::errorChanged, this, &CreateSoundMessageWidget::displayRecorderError);
    initializeInput();
    updateRecorderState(mAudioRecorder->recorderState());
}

CreateSoundMessageWidget::~CreateSoundMessageWidget() = default;

void CreateSoundMessageWidget::displayRecorderError()
{
    if (mAudioRecorder->error() != QMediaRecorder::NoError) {
        mMessageWidget->setText(mAudioRecorder->errorString());
        mMessageWidget->animatedShow();
    }
}

void CreateSoundMessageWidget::initializeInput()
{
    mDeviceComboBox->addItem(i18n("Default"), QVariant(QString()));
    for (auto device : QMediaDevices::audioInputs()) {
        auto name = device.description();
        mDeviceComboBox->addItem(name, QVariant::fromValue(device));
    }

    mTemporaryFile = new QTemporaryFile(QDir::tempPath() + QLatin1String("/ruqola_XXXXXX")); // TODO fix extension
    mTemporaryFile->setAutoRemove(false);
    mTemporaryFile->open();
    //        QMediaFormat format;
    //        format.setFileFormat(QMediaFormat::FileFormat::AVI);
    //        mMediaRecorder->setMediaFormat(format);
    // Define url temporary file.
    mAudioRecorder->setOutputLocation(QUrl::fromLocalFile(mTemporaryFile->fileName()));
    // qDebug() << " store " << mTemporaryFile->fileName();
}

void CreateSoundMessageWidget::updateRecordTime(qint64 duration)
{
    const QString str = i18n("Recorded %1 sec", duration / 1000);
    mLabelDuration->setText(str);
}

QUrl CreateSoundMessageWidget::temporaryFilePath() const
{
    // qDebug() << "output location" << mMediaRecorder->outputLocation() << " dd " << mMediaRecorder->actualLocation();
    return mAudioRecorder->actualLocation();
}

void CreateSoundMessageWidget::stop()
{
    mAudioRecorder->stop();
    Q_EMIT recordDone();
}

void CreateSoundMessageWidget::record()
{
    mCaptureSession.audioInput()->setDevice(mDeviceComboBox->itemData(mDeviceComboBox->currentIndex()).value<QAudioDevice>());
    mAudioRecorder->record();
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

#include "moc_createsoundmessagewidget.cpp"
