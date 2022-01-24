/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuswidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QToolButton>

UploadFileProgressStatusWidget::UploadFileProgressStatusWidget(QWidget *parent)
    : QWidget(parent)
    , mFileName(new QLabel(this))
    , mProgressBar(new QProgressBar(this))
    , mCancelToolButton(new QToolButton(this))
{
    auto hboxLayout = new QHBoxLayout(this);
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});

    mFileName->setObjectName(QStringLiteral("mFileName"));
    hboxLayout->addWidget(mFileName);

    mProgressBar->setObjectName(QStringLiteral("mProgressBar"));
    mProgressBar->setRange(0, 100);

    mCancelToolButton->setObjectName(QStringLiteral("mCancelToolButton"));
    mCancelToolButton->setIcon(QIcon::fromTheme(QStringLiteral("dialog-cancel")));
    connect(mCancelToolButton, &QToolButton::clicked, this, &UploadFileProgressStatusWidget::cancelUpload);

    hboxLayout->addWidget(mProgressBar);
}

UploadFileProgressStatusWidget::~UploadFileProgressStatusWidget() = default;

void UploadFileProgressStatusWidget::setUploadFileName(const QString &str)
{
    mFileName->setText(str);
}

void UploadFileProgressStatusWidget::setValue(int value)
{
    mProgressBar->setValue(value);
}
