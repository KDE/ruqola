/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagedialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KStandardAction>
#include <QDialogButtonBox>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myShowImageDialogGroupName[] = "ShowImageDialog";
}
ShowImageDialog::ShowImageDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowImageWidget(new ShowImageWidget(account, this))
    , mClipboardMenu(new QMenu(this))
{
    setWindowTitle(i18nc("@title:window", "Display Image"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowImageWidget->setObjectName(QStringLiteral("mShowImageWidget"));
    mainLayout->addWidget(mShowImageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close | QDialogButtonBox::Save, this);
    buttonBox->setObjectName(QStringLiteral("button"));

    mClipboardImageAction = KStandardAction::copy(mShowImageWidget, &ShowImageWidget::copyImage, this);
    mClipboardImageAction->setObjectName(QStringLiteral("clipboardLocationAction"));
    mClipboardImageAction->setText(i18n("Copy Image to Clipboard"));

    auto clipboardLocationAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy Location to Clipboard"), this);
    clipboardLocationAction->setObjectName(QStringLiteral("clipboardLocationAction"));
    connect(clipboardLocationAction, &QAction::triggered, mShowImageWidget, &ShowImageWidget::copyLocation);

    mClipboardMenu->setObjectName(QStringLiteral("mClipboardMenu"));
    mClipboardMenu->addAction(mClipboardImageAction);
    mClipboardMenu->addAction(clipboardLocationAction);

    auto clipboardButton = new QToolButton(this);
    clipboardButton->setObjectName(QStringLiteral("clipboardButton"));

    clipboardButton->setMenu(mClipboardMenu);
    clipboardButton->setPopupMode(QToolButton::MenuButtonPopup);
    clipboardButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    buttonBox->addButton(clipboardButton, QDialogButtonBox::ActionRole);
    clipboardButton->setDefaultAction(clipboardLocationAction);

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
    if (info.isAnimatedImage) {
        mClipboardImageAction->setEnabled(false);
    }
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
