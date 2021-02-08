/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "showvideodialog.h"
#include "showvideowidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

#include <QDialogButtonBox>
#include <QUrl>
#include <QVBoxLayout>

namespace
{
static const char myShowVideoDialogGroupName[] = "ShowVideoDialog";
}

ShowVideoDialog::ShowVideoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Video"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowVideoWidget = new ShowVideoWidget(this);
    mShowVideoWidget->setObjectName(QStringLiteral("mShowVideoWidget"));
    mainLayout->addWidget(mShowVideoWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ShowVideoDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

ShowVideoDialog::~ShowVideoDialog()
{
    writeConfig();
}

void ShowVideoDialog::setVideoUrl(const QUrl &videoPath)
{
    mShowVideoWidget->setVideoUrl(videoPath);
    setWindowTitle(i18nc("@title:window", "Video: %1", videoPath.fileName()));
}

void ShowVideoDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myShowVideoDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowVideoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myShowVideoDialogGroupName);
    group.writeEntry("Size", size());
}
