/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuswidget.h"

#include <KLocalizedString>

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
    mCancelToolButton->setToolTip(i18n("Cancel Upload"));
    connect(mCancelToolButton, &QToolButton::clicked, this, [this] {
        Q_EMIT cancelUpload(mIdentifier);
    });

    hboxLayout->addWidget(mProgressBar);
    hboxLayout->addWidget(mCancelToolButton);
}

UploadFileProgressStatusWidget::~UploadFileProgressStatusWidget() = default;

void UploadFileProgressStatusWidget::setUploadFileName(const QString &str)
{
    mFileName->setText(str);
}

int UploadFileProgressStatusWidget::identifier() const
{
    return mIdentifier;
}

void UploadFileProgressStatusWidget::setIdentifier(int newIdentifier)
{
    mIdentifier = newIdentifier;
}

void UploadFileProgressStatusWidget::setValue(int value)
{
    mProgressBar->setValue(value);
}

#include "moc_uploadfileprogressstatuswidget.cpp"
