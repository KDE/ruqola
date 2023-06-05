/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidget.h"
#include <KMessageWidget>
#include <QAudioInput>
#include <QCamera>
#include <QCameraDevice>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVideoWidget>

#include <KLocalizedString>
#include <QDir>
#include <QMediaFormat>

CreateVideoMessageWidget::CreateVideoMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mVideoWidget(new QVideoWidget(this))
    , mErrorLabel(new QLabel(this))
    , mCamera(new QCamera(this))
    , mListCamera(new QComboBox(this))
    , mRecordButton(new QToolButton(this))
    , mPauseButton(new QToolButton(this))
    , mStopButton(new QToolButton(this))
    , mDurationLabel(new QLabel(this))
    , mMessageWidget(new KMessageWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mMessageWidget->setObjectName(QStringLiteral("mMessageWidget"));
    mainLayout->addWidget(mMessageWidget);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Information);
    mMessageWidget->setWordWrap(true);

    mListCamera->setObjectName(QStringLiteral("mListCamera"));
    mainLayout->addWidget(mListCamera);

    mVideoWidget->setObjectName(QStringLiteral("mVideoWidget"));
    mainLayout->addWidget(mVideoWidget, 1);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});

    mStopButton->setObjectName(QStringLiteral("mStopButton"));
    mStopButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
    hboxLayout->addWidget(mStopButton);
    connect(mStopButton, &QToolButton::clicked, this, &CreateVideoMessageWidget::stop);

    mPauseButton->setObjectName(QStringLiteral("mPauseButton"));
    mPauseButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-pause")));
    hboxLayout->addWidget(mPauseButton);
    connect(mPauseButton, &QToolButton::clicked, this, &CreateVideoMessageWidget::pause);

    mRecordButton->setObjectName(QStringLiteral("mRecordButton"));
    mRecordButton->setIcon(QIcon::fromTheme(QStringLiteral("media-record")));
    hboxLayout->addWidget(mRecordButton);
    connect(mRecordButton, &QToolButton::clicked, this, &CreateVideoMessageWidget::record);

    mDurationLabel->setObjectName(QStringLiteral("mDurationLabel"));
    hboxLayout->addWidget(mDurationLabel);
    mainLayout->addLayout(hboxLayout);

    mErrorLabel->setObjectName(QStringLiteral("mErrorLabel"));
    mErrorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mainLayout->addWidget(mErrorLabel);
    mErrorLabel->setVisible(false);
    updateCameras();
    setCamera(QMediaDevices::defaultVideoInput());
}

CreateVideoMessageWidget::~CreateVideoMessageWidget() = default;

QUrl CreateVideoMessageWidget::temporaryFilePath() const
{
    // qDebug() << " XCCCCCCCCCCCCCCCCCCC" << mMediaRecorder->outputLocation() << " dd " << mMediaRecorder->actualLocation();
    return mMediaRecorder->actualLocation();
}

void CreateVideoMessageWidget::updateCameras()
{
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : availableCameras) {
        mListCamera->addItem(cameraDevice.description(), QVariant::fromValue(cameraDevice));
    }
}

void CreateVideoMessageWidget::setCamera(const QCameraDevice &cameraDevice)
{
    mCamera.reset(new QCamera(cameraDevice));
    mCaptureSession.setCamera(mCamera.data());

    mAudioInput.reset(new QAudioInput);
    mCaptureSession.setAudioInput(mAudioInput.get());

    connect(mCamera.data(), &QCamera::activeChanged, this, &CreateVideoMessageWidget::updateCameraActive);
    connect(mCamera.data(), &QCamera::errorOccurred, this, &CreateVideoMessageWidget::displayCameraError);

    if (!mMediaRecorder) {
        mMediaRecorder.reset(new QMediaRecorder);
        mTemporaryFile = new QTemporaryFile(QDir::tempPath() + QLatin1String("/ruqola_XXXXXX")); // TODO fix extension
        mTemporaryFile->setAutoRemove(false);
        mTemporaryFile->open();
        //        QMediaFormat format;
        //        format.setFileFormat(QMediaFormat::FileFormat::AVI);
        //        mMediaRecorder->setMediaFormat(format);
        // Define url temporary file.
        mMediaRecorder->setOutputLocation(QUrl::fromLocalFile(mTemporaryFile->fileName()));
        qDebug() << " store " << mTemporaryFile->fileName();
        mCaptureSession.setRecorder(mMediaRecorder.data());
        connect(mMediaRecorder.data(), &QMediaRecorder::recorderStateChanged, this,
                &CreateVideoMessageWidget::updateRecorderState);
        connect(mMediaRecorder.data(), &QMediaRecorder::durationChanged, this,
                &CreateVideoMessageWidget::updateRecordTime);
        connect(mMediaRecorder.data(), &QMediaRecorder::errorChanged, this,
                &CreateVideoMessageWidget::displayRecorderError);
    }
    mCaptureSession.setVideoOutput(mVideoWidget);
    updateCameraActive(mCamera->isActive());
    updateRecorderState(mMediaRecorder->recorderState());

    mCamera->start();
}

void CreateVideoMessageWidget::displayCameraError()
{
    if (mCamera->error() != QCamera::NoError) {
        mMessageWidget->setText(mCamera->errorString());
        mMessageWidget->animatedShow();
    }
}

void CreateVideoMessageWidget::updateRecorderState(QMediaRecorder::RecorderState state)
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

void CreateVideoMessageWidget::updateCameraActive(bool active)
{
    // TODO update actions
}

void CreateVideoMessageWidget::displayRecorderError()
{
    if (mMediaRecorder->error() != QMediaRecorder::NoError) {
        mMessageWidget->setText(mMediaRecorder->errorString());
        mMessageWidget->animatedShow();
    }
}

void CreateVideoMessageWidget::record()
{
    mMediaRecorder->record();
    updateRecordTime();
}

void CreateVideoMessageWidget::pause()
{
    mMediaRecorder->pause();
}

void CreateVideoMessageWidget::stop()
{
    mMediaRecorder->stop();
}

void CreateVideoMessageWidget::updateRecordTime()
{
    const QString str = i18n("Recorded %1 sec", mMediaRecorder->duration() / 1000);
    mDurationLabel->setText(str);
}
