/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfiledialog.h"
#include "uploadfilewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myUploadFileDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void UploadFileDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myUploadFileDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

UploadFileDialog::UploadFileInfo UploadFileDialog::fileInfo() const
{
    UploadFileInfo result;
    result.description = mUploadFileWidget->description();
    result.fileUrl = mUploadFileWidget->fileUrl();
    result.fileName = mUploadFileWidget->fileName();
    return result;
}

void UploadFileDialog::setPixmap(const QPixmap &pix)
{
    if (!pix.isNull()) {
        setWindowTitle(i18nc("@title:window", "Upload Image"));
    }
    mUploadFileWidget->setPixmap(pix);
}

#include "moc_uploadfiledialog.cpp"
