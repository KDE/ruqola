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
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

CreateVideoMessageWidget::CreateVideoMessageWidget(QWidget *parent)
    : QWidget(parent)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , mVideoWidget(new QVideoWidget(this))
#endif
    , mErrorLabel(new QLabel(this))
    , mCamera(new QCamera(this))
    , mListCamera(new QComboBox(this))
    , mRecordButton(new QToolButton(this))
    , mDurationLabel(new QLabel(QStringLiteral("00:00"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mListCamera->setObjectName(QStringLiteral("mListCamera"));
    mainLayout->addWidget(mListCamera);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mVideoWidget->setObjectName(QStringLiteral("mVideoWidget"));
    mainLayout->addWidget(mVideoWidget, 1);
#endif

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
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
}

CreateVideoMessageWidget::~CreateVideoMessageWidget() = default;

void CreateVideoMessageWidget::updateCameras()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : availableCameras) {
        mListCamera->addItem(cameraDevice.description(), QVariant::fromValue(cameraDevice));
        //        QAction *videoDeviceAction = new QAction(cameraDevice.description(), videoDevicesGroup);
        //        videoDeviceAction->setCheckable(true);
        //        videoDeviceAction->setData(QVariant::fromValue(cameraDevice));
        //        if (cameraDevice == QMediaDevices::defaultVideoInput())
        //            videoDeviceAction->setChecked(true);
    }
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void CreateVideoMessageWidget::setCamera(const QCameraDevice &cameraDevice)
{
#if 0
    mCamera.reset(new QCamera(cameraDevice));
    m_captureSession.setCamera(mCamera.data());

    connect(mCamera.data(), &QCamera::activeChanged, this, &CreateVideoMessageWidget::updateCameraActive);
    connect(mCamera.data(), &QCamera::errorOccurred, this, &CreateVideoMessageWidget::displayCameraError);

    if (!mMediaRecorder) {
        mMediaRecorder.reset(new QMediaRecorder);
        m_captureSession.setRecorder(mMediaRecorder.data());
        connect(mMediaRecorder.data(), &QMediaRecorder::recorderStateChanged, this,
                &CreateVideoMessageWidget::updateRecorderState);
        connect(mMediaRecorder.data(), &QMediaRecorder::durationChanged, this,
                &CreateVideoMessageWidget::updateRecordTime);
        connect(mMediaRecorder.data(), &QMediaRecorder::errorChanged, this,
                &CreateVideoMessageWidget::displayRecorderError);
    }

    updateCameraActive(mCamera->isActive());
    updateRecorderState(mMediaRecorder->recorderState());

    updateCaptureMode();

    mCamera->start();
#endif
}
#endif

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
    // mMediaRecorder->record();
    // updateRecordTime();
#endif
}

void CreateVideoMessageWidget::pause()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // mMediaRecorder->pause();
#endif
}

void CreateVideoMessageWidget::stop()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // mMediaRecorder->stop();
#endif
}
