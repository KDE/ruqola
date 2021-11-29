/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "showimagedialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myShowImageDialogGroupName[] = "ShowImageDialog";
}
ShowImageDialog::ShowImageDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowImageWidget(new ShowImageWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Display Image"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowImageWidget->setObjectName(QStringLiteral("mShowImageWidget"));
    mainLayout->addWidget(mShowImageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close | QDialogButtonBox::Save, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ShowImageDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, mShowImageWidget, &ShowImageWidget::saveAs);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

ShowImageDialog::~ShowImageDialog()
{
    writeConfig();
}

void ShowImageDialog::setImageInfo(const ShowImageWidget::ImageInfo &info)
{
    mShowImageWidget->setImageInfo(info);
}

void ShowImageDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowImageDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowImageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowImageDialogGroupName);
    group.writeEntry("Size", size());
}
