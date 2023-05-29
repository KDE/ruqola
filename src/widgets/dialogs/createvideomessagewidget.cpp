/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidget.h"
#include <KMessageWidget>
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

CreateVideoMessageWidget::CreateVideoMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mVideoWidget(new QVideoWidget(this))
    , mErrorLabel(new QLabel(this))
    , mCamera(new QCamera(this))
    , mListCamera(new QComboBox(this))
    , mRecordButton(new QToolButton(this))
    , mPauseButton(new QToolButton(this))
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

    mPauseButton->setObjectName(QStringLiteral("mPauseButton"));
    hboxLayout->addWidget(mPauseButton);

    mRecordButton->setObjectName(QStringLiteral("mRecordButton"));
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

    connect(mCamera.data(), &QCamera::activeChanged, this, &CreateVideoMessageWidget::updateCameraActive);
    connect(mCamera.data(), &QCamera::errorOccurred, this, &CreateVideoMessageWidget::displayCameraError);

    if (!mMediaRecorder) {
        mMediaRecorder.reset(new QMediaRecorder);
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
#if 0
    switch (state) {
    case QMediaRecorder::StoppedState:
        ui->recordButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->metaDataButton->setEnabled(true);
        break;
    case QMediaRecorder::PausedState:
        ui->recordButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        ui->metaDataButton->setEnabled(false);
        break;
    case QMediaRecorder::RecordingState:
        ui->recordButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        ui->metaDataButton->setEnabled(false);
        break;
    }
#endif
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

void CreateVideoMessageWidget::startCamera()
{
    mCamera->start();
}

void CreateVideoMessageWidget::stopCamera()
{
    mCamera->stop();
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
