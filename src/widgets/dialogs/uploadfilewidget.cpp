/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilewidget.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KUrlRequester>

#include <KLineEdit>
#include <QFormLayout>
#include <QLabel>

UploadFileWidget::UploadFileWidget(QWidget *parent)
    : QWidget(parent)
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
    , mImagePreview(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mImagePreview->setObjectName(QStringLiteral("mImagePreview"));
    mainLayout->addWidget(mImagePreview, 0, Qt::AlignCenter);
    mImagePreview->hide(); // Hide by default

    auto layout = new QFormLayout;
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});
    mainLayout->addLayout(layout);

    mFileName->setObjectName(QStringLiteral("mFileName"));
    mFileName->setClearButtonEnabled(true);
    layout->addRow(i18n("File Name:"), mFileName);
    KLineEditEventHandler::catchReturnKey(mFileName);

    mDescription->setObjectName(QStringLiteral("mDescription"));
    mDescription->setClearButtonEnabled(true);
    layout->addRow(i18n("Description:"), mDescription);
    KLineEditEventHandler::catchReturnKey(mDescription);
}

UploadFileWidget::~UploadFileWidget() = default;

QString UploadFileWidget::description() const
{
    return mDescription->text();
}

QString UploadFileWidget::fileName() const
{
    return mFileName->text();
}

QUrl UploadFileWidget::fileUrl() const
{
    return mUrl;
}

void UploadFileWidget::setFileUrl(const QUrl &url)
{
    mUrl = url;
    const QFileInfo fileInfo(mUrl.toLocalFile());
    mFileName->setText(fileInfo.fileName());
}

void UploadFileWidget::setPixmap(const QPixmap &pix)
{
    if (!pix.isNull()) {
        mImagePreview->setVisible(true);
        mImagePreview->setPixmap(pix);
    }
}

#include "moc_uploadfilewidget.cpp"
