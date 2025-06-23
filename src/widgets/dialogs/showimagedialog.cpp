/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagedialog.h"
using namespace Qt::Literals::StringLiterals;

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KStandardActions>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWindow>

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowImageWidget->setObjectName(u"mShowImageWidget"_s);
    mainLayout->addWidget(mShowImageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close | QDialogButtonBox::Save, this);
    buttonBox->setObjectName(u"button"_s);

    mClipboardImageAction = KStandardActions::copy(mShowImageWidget, &ShowImageWidget::copyImage, this);
    mClipboardImageAction->setObjectName(u"clipboardLocationAction"_s);
    mClipboardImageAction->setText(i18n("Copy Image to Clipboard"));

    auto clipboardLocationAction = new QAction(QIcon::fromTheme(u"edit-copy"_s), i18n("Copy Location to Clipboard"), this);
    clipboardLocationAction->setObjectName(u"clipboardLocationAction"_s);
    connect(clipboardLocationAction, &QAction::triggered, mShowImageWidget, &ShowImageWidget::copyLocation);

    mClipboardMenu->setObjectName(u"mClipboardMenu"_s);
    mClipboardMenu->addAction(mClipboardImageAction);
    mClipboardMenu->addAction(clipboardLocationAction);

    auto clipboardButton = new QToolButton(this);
    clipboardButton->setObjectName(u"clipboardButton"_s);

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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowImageDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowImageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowImageDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ShowImageDialog::showImages(const QByteArray &fileId, const QByteArray &roomId)
{
    mShowImageWidget->showImages(fileId, roomId);
}
#include "moc_showimagedialog.cpp"
