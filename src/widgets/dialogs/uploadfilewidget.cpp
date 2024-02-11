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
    , mDescription(new QLineEdit(this))
    , mSelectFile(new KUrlRequester(this))
    , mImagePreview(new QLabel(this))
    , mFileLabel(new QLabel(i18n("File:"), this))
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

    mDescription->setObjectName(QStringLiteral("mDescription"));
    mDescription->setClearButtonEnabled(true);
    layout->addRow(i18n("Description:"), mDescription);
    KLineEditEventHandler::catchReturnKey(mDescription);

    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));
    mFileLabel->setObjectName(QStringLiteral("mFileLabel"));
    layout->addRow(mFileLabel, mSelectFile);

    connect(mSelectFile->lineEdit(), &KLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT updateOkButton(!str.trimmed().isEmpty());
    });
}

UploadFileWidget::~UploadFileWidget() = default;

QString UploadFileWidget::description() const
{
    return mDescription->text();
}

QUrl UploadFileWidget::fileUrl() const
{
    return mSelectFile->url();
}

void UploadFileWidget::setFileUrl(const QUrl &url)
{
    mSelectFile->setUrl(url);
}

void UploadFileWidget::setPixmap(const QPixmap &pix)
{
    if (!pix.isNull()) {
        mSelectFile->setVisible(false);
        mFileLabel->setVisible(false);
        mImagePreview->setVisible(true);
        mImagePreview->setPixmap(pix);
    }
}

void UploadFileWidget::setAuthorizedMediaTypes(const QStringList &mediaTypes)
{
    mSelectFile->setMimeTypeFilters(mediaTypes);
}

#include "moc_uploadfilewidget.cpp"
