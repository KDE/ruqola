/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilewidget.h"
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
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mImagePreview->setObjectName(QStringLiteral("mImagePreview"));
    mainLayout->addWidget(mImagePreview, 0, Qt::AlignCenter);
    mImagePreview->hide(); // Hide by default

    mainLayout->addStretch(1);

    auto layout = new QFormLayout;
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});
    mainLayout->addLayout(layout);

    mMimeTypeLabel->setObjectName(QStringLiteral("mMimeTypeLabel"));

    mFileNameInfo->setObjectName(QStringLiteral("mFileNameInfo"));
    layout->addRow(mMimeTypeLabel, mFileNameInfo);

    mFileName->setObjectName(QStringLiteral("mFileName"));
    mFileName->setClearButtonEnabled(true);
    layout->addRow(i18n("File Name:"), mFileName);
    KLineEditEventHandler::catchReturnKey(mFileName);

    mDescription->setObjectName(QStringLiteral("mDescription"));
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
    mFileNameInfo->setText(QStringLiteral("%1 - %2").arg(fileInfo.fileName(), KFormat().formatByteSize(fileInfo.size())));

    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileInfo);
    const QPixmap pixmapMimetype =
        QIcon::fromTheme(mimeType.iconName(), QIcon::fromTheme(QStringLiteral("unknown"))).pixmap(style()->pixelMetric(QStyle::PM_LargeIconSize));
    mMimeTypeLabel->setPixmap(pixmapMimetype);
    const QPixmap pixmap(mUrl.toLocalFile());
    if (!pixmap.isNull()) {
        setPixmap(pixmap);
    }
}

void UploadFileWidget::setPixmap(const QPixmap &pix)
{
    if (!pix.isNull()) {
        mImagePreview->setVisible(true);
        const QSize s = QSize(600, 800) * screen()->devicePixelRatio();
        const QPixmap p = pix.scaled(s, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        mImagePreview->setMaximumSize(s);
        mImagePreview->setPixmap(p);
        Q_EMIT uploadImage();
    }
}

#include "moc_uploadfilewidget.cpp"
