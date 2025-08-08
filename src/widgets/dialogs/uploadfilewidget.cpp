/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KUrlRequester>

#include <KFormat>
#include <KLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QMimeDatabase>
#include <QScreen>
#include <QStyle>

UploadFileWidget::UploadFileWidget(QWidget *parent)
    : QWidget(parent)
    , mFileName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
    , mImagePreview(new QLabel(this))
    , mFileNameInfo(new QLabel(this))
    , mMimeTypeLabel(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mImagePreview->setObjectName(u"mImagePreview"_s);
    mainLayout->addWidget(mImagePreview);
    mImagePreview->hide(); // Hide by default
    mImagePreview->setScaledContents(true);
    mImagePreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto layout = new QFormLayout;
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins({});
    mainLayout->addLayout(layout);

    mMimeTypeLabel->setObjectName(u"mMimeTypeLabel"_s);

    mFileNameInfo->setObjectName(u"mFileNameInfo"_s);
    layout->addRow(mMimeTypeLabel, mFileNameInfo);

    mFileName->setObjectName(u"mFileName"_s);
    mFileName->setClearButtonEnabled(true);
    layout->addRow(i18n("File Name:"), mFileName);
    KLineEditEventHandler::catchReturnKey(mFileName);

    mDescription->setObjectName(u"mDescription"_s);
    mDescription->setClearButtonEnabled(true);
    mDescription->setFocus();
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
    mFileNameInfo->setText(u"%1 - %2"_s.arg(fileInfo.fileName(), KFormat().formatByteSize(fileInfo.size())));

    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileInfo);
    const QPixmap pixmapMimetype = QIcon::fromTheme(mimeType.iconName(), QIcon::fromTheme(u"unknown"_s)).pixmap(style()->pixelMetric(QStyle::PM_LargeIconSize));
    mMimeTypeLabel->setPixmap(pixmapMimetype);
    const QPixmap pixmap(mUrl.toLocalFile());
    if (!pixmap.isNull()) {
        setPixmap(pixmap);
    }
}

void UploadFileWidget::setPixmap(const QPixmap &pix)
{
    if (!pix.isNull()) {
        mPix = pix;
        mImagePreview->setVisible(true);
        const QSize s = QSize(200, 200) * screen()->devicePixelRatio();
        const QPixmap p = mPix.scaled(s, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        mImagePreview->setMinimumSize(s);
        mImagePreview->setPixmap(p);
        Q_EMIT uploadImage();
    }
}

void UploadFileWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!mImagePreview->pixmap().isNull()) {
        const QPixmap pixmap = mPix.scaled(mImagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        mImagePreview->setPixmap(pixmap);
    }
}

#include "moc_uploadfilewidget.cpp"
