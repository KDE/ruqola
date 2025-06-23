/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    hboxLayout->setObjectName(u"hboxLayout"_s);
    hboxLayout->setContentsMargins({});

    mFileName->setObjectName(u"mFileName"_s);
    hboxLayout->addWidget(mFileName);

    mProgressBar->setObjectName(u"mProgressBar"_s);
    mProgressBar->setRange(0, 100);
    mProgressBar->setFormat(i18nc("Percent value; %p is the value, % is the percent sign", "%p%"));

    mCancelToolButton->setObjectName(u"mCancelToolButton"_s);
    mCancelToolButton->setIcon(QIcon::fromTheme(u"dialog-cancel"_s));
    mCancelToolButton->setToolTip(i18nc("@info:tooltip", "Cancel Upload"));
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
