/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideodialog.h"
#include "showvideowidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
static const char myShowVideoDialogGroupName[] = "ShowVideoDialog";
}

ShowVideoDialog::ShowVideoDialog(QWidget *parent)
    : QDialog(parent)
    , mShowVideoWidget(new ShowVideoWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Video"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

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
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowVideoDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowVideoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowVideoDialogGroupName);
    group.writeEntry("Size", size());
}
