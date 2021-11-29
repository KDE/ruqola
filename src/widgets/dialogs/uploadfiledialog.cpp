/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfiledialog.h"
#include "uploadfilewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myUploadFileDialogGroupName[] = "UploadFileDialog";
}
UploadFileDialog::UploadFileDialog(QWidget *parent)
    : QDialog(parent)
    , mUploadFileWidget(new UploadFileWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Upload File"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mUploadFileWidget->setObjectName(QStringLiteral("mUploadFileWidget"));
    mainLayout->addWidget(mUploadFileWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &UploadFileDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &UploadFileDialog::reject);
    mainLayout->addWidget(buttonBox);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mUploadFileWidget, &UploadFileWidget::updateOkButton, mOkButton, &QPushButton::setEnabled);
    readConfig();
}

UploadFileDialog::~UploadFileDialog()
{
    writeConfig();
}

void UploadFileDialog::setFileUrl(const QUrl &url)
{
    mUploadFileWidget->setFileUrl(url);
}

void UploadFileDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myUploadFileDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void UploadFileDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myUploadFileDialogGroupName);
    group.writeEntry("Size", size());
}

UploadFileDialog::UploadFileInfo UploadFileDialog::fileInfo() const
{
    UploadFileInfo result;
    result.description = mUploadFileWidget->description();
    result.fileUrl = mUploadFileWidget->fileUrl();
    return result;
}
