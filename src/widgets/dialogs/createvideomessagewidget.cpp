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
#include <QLabel>
#include <QVBoxLayout>

CreateVideoMessageWidget::CreateVideoMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mErrorLabel(new QLabel(this))
    , mCamera(new QCamera(this))
    , mListCamera(new QComboBox(this))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , mVideoWidget(new QVideoWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mListCamera->setObjectName(QStringLiteral("mListCamera"));
    mainLayout->addWidget(mListCamera);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mVideoWidget->setObjectName(QStringLiteral("mVideoWidget"));
    mainLayout->addWidget(mVideoWidget);
#endif

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

    connect(mCamera.data(), &QCamera::activeChanged, this, &Camera::updateCameraActive);
    connect(mCamera.data(), &QCamera::errorOccurred, this, &Camera::displayCameraError);

    if (!m_mediaRecorder) {
        m_mediaRecorder.reset(new QMediaRecorder);
        m_captureSession.setRecorder(m_mediaRecorder.data());
        connect(m_mediaRecorder.data(), &QMediaRecorder::recorderStateChanged, this,
                &Camera::updateRecorderState);
        connect(m_mediaRecorder.data(), &QMediaRecorder::durationChanged, this,
                &Camera::updateRecordTime);
        connect(m_mediaRecorder.data(), &QMediaRecorder::errorChanged, this,
                &Camera::displayRecorderError);
    }

    updateCameraActive(mCamera->isActive());
    updateRecorderState(m_mediaRecorder->recorderState());

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
