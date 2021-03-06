/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "createvideomessagewidget.h"
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
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

    mCamera->setObjectName(QStringLiteral("mCamera"));

    auto viewfinder = new QCameraViewfinder(this);
    viewfinder->show();

    mCamera->setViewfinder(viewfinder);

    auto imageCapture = new QCameraImageCapture(mCamera, this);

    mCamera->setCaptureMode(QCamera::CaptureStillImage);
    mCamera->start();

    mErrorLabel->setObjectName(QStringLiteral("mErrorLabel"));
    mErrorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mainLayout->addWidget(mErrorLabel);
    mErrorLabel->setVisible(false);
}

CreateVideoMessageWidget::~CreateVideoMessageWidget()
{
}

void CreateVideoMessageWidget::startCamera()
{
    mCamera->start();
}

void CreateVideoMessageWidget::stopCamera()
{
    mCamera->stop();
}
