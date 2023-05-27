/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidget.h"
#include <QCamera>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#else
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMediaRecorder>
#include <QVideoWidget>
#endif
#include <KMessageWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

#include <KLocalizedString>

CreateVideoMessageWidget::CreateVideoMessageWidget(QWidget *parent)
    : QWidget(parent)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , mVideoWidget(new QVideoWidget(this))
#endif
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mVideoWidget->setObjectName(QStringLiteral("mVideoWidget"));
    mainLayout->addWidget(mVideoWidget, 1);
#endif

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});

    mPauseButton->setObjectName(QStringLiteral("mPauseButton"));
    hboxLayout->addWidget(mPauseButton);

    mRecordButton->setObjectName(QStringLiteral("mRecordButton"));
    hboxLayout->addWidget(mRecordButton);

    mDurationLabel->setObjectName(QStringLiteral("mDurationLabel"));
    hboxLayout->addWidget(mDurationLabel);
    mainLayout->addLayout(hboxLayout);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    mCamera->setObjectName(QStringLiteral("mCamera"));

    auto viewfinder = new QCameraViewfinder(this);
    viewfinder->show();

    mCamera->setViewfinder(viewfinder);
    auto imageCapture = new QCameraImageCapture(mCamera, this);

    mCamera->setCaptureMode(QCamera::CaptureStillImage);
    mCamera->start();
#endif

    mErrorLabel->setObjectName(QStringLiteral("mErrorLabel"));
    mErrorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mainLayout->addWidget(mErrorLabel);
    mErrorLabel->setVisible(false);
    updateCameras();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    setCamera(QMediaDevices::defaultVideoInput());
#endif
}

CreateVideoMessageWidget::~CreateVideoMessageWidget() = default;

void CreateVideoMessageWidget::updateCameras()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : availableCameras) {
        mListCamera->addItem(cameraDevice.description(), QVariant::fromValue(cameraDevice));
    }
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
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

    updateCameraActive(mCamera->isActive());
    updateRecorderState(mMediaRecorder->recorderState());

    mCamera->start();
}
#endif

void CreateVideoMessageWidget::displayCameraError()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (mCamera->error() != QCamera::NoError) {
        mMessageWidget->setText(mCamera->errorString());
        mMessageWidget->animatedShow();
    }
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
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
#endif

void CreateVideoMessageWidget::updateCameraActive(bool active)
{
}

void CreateVideoMessageWidget::displayRecorderError()
{
    // TODO
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mMediaRecorder->record();
    updateRecordTime();
#endif
}

void CreateVideoMessageWidget::pause()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mMediaRecorder->pause();
#endif
}

void CreateVideoMessageWidget::stop()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mMediaRecorder->stop();
#endif
}

void CreateVideoMessageWidget::updateRecordTime()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QString str = i18n("Recorded %1 sec", mMediaRecorder->duration() / 1000);
    mDurationLabel->setText(str);
#endif
}
