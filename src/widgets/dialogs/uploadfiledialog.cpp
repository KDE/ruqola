/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfiledialog.h"
using namespace Qt::Literals::StringLiterals;

#include "uploadfilewidget.h"
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

UploadFileDialog::UploadFileDialog(QWidget *parent)
    : QDialog(parent)
    , mUploadFileWidget(new UploadFileWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Upload File"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mUploadFileWidget->setObjectName(u"mUploadFileWidget"_s);
    mainLayout->addWidget(mUploadFileWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &UploadFileDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &UploadFileDialog::reject);
    connect(mUploadFileWidget, &UploadFileWidget::uploadImage, this, [this]() {
        setWindowTitle(i18nc("@title:window", "Upload Image"));
    });
    mainLayout->addWidget(buttonBox);
}

UploadFileDialog::~UploadFileDialog() = default;

void UploadFileDialog::setFileUrl(const QUrl &url)
{
    mUploadFileWidget->setFileUrl(url);
}

UploadFileDialog::UploadFileInfo UploadFileDialog::fileInfo() const
{
    UploadFileInfo result;
    result.description = mUploadFileWidget->description();
    result.fileUrl = mUploadFileWidget->fileUrl();
    result.fileName = mUploadFileWidget->fileName();
    return result;
}

#include "moc_uploadfiledialog.cpp"
