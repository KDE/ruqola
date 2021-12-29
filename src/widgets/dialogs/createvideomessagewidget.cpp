/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidget.h"
#include <QCamera>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#else
#include <QImageCapture>
#endif
#include <QLabel>
#include <QVBoxLayout>

CreateVideoMessageWidget::CreateVideoMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mErrorLabel(new QLabel(this))
    , mCamera(new QCamera(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

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
}

CreateVideoMessageWidget::~CreateVideoMessageWidget() = default;

void CreateVideoMessageWidget::startCamera()
{
    mCamera->start();
}

void CreateVideoMessageWidget::stopCamera()
{
    mCamera->stop();
}
