/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "uploadfiledialog.h"
#include "uploadfilewidget.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QDialogButtonBox>

UploadFileDialog::UploadFileDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Upload File"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mUploadFileWidget = new UploadFileWidget(this);
    mUploadFileWidget->setObjectName(QStringLiteral("mUploadFileWidget"));
    mainLayout->addWidget(mUploadFileWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &UploadFileDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &UploadFileDialog::reject);
    mainLayout->addWidget(buttonBox);
}

UploadFileDialog::~UploadFileDialog()
{

}

UploadFileDialog::UploadFileInfo UploadFileDialog::fileInfo() const
{
    UploadFileInfo result;
    result.description = mUploadFileWidget->description();
    result.message = mUploadFileWidget->message();
    result.fileUrl = mUploadFileWidget->fileUrl();
    return result;
}
