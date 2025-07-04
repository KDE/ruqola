/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagedialog.h"
#include "rocketchataccount.h"
#include <KApplicationTrader>
#include <QMimeDatabase>

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
using namespace Qt::Literals::StringLiterals;
ShowImageDialog::ShowImageDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowImageWidget(new ShowImageWidget(account, this))
    , mClipboardMenu(new QMenu(this))
    , mOpenWithButton(new QToolButton(this))
    , mOpenWithMenu(new QMenu(this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Display Image"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowImageWidget->setObjectName(u"mShowImageWidget"_s);
    mainLayout->addWidget(mShowImageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close | QDialogButtonBox::Save, this);
    buttonBox->setObjectName(u"button"_s);

    mOpenWithButton->setObjectName(u"openWithButton"_s);
    mOpenWithButton->setPopupMode(QToolButton::MenuButtonPopup);
    mOpenWithButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    buttonBox->addButton(mOpenWithButton, QDialogButtonBox::ActionRole);
    mOpenWithMenu->setObjectName(u"mOpenWithMenu"_s);
    mOpenWithButton->setMenu(mOpenWithMenu);
    connect(mOpenWithMenu, &QMenu::triggered, this, &ShowImageDialog::slotOpenWith);

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

    connect(mShowImageWidget, &ShowImageWidget::updateMenu, this, &ShowImageDialog::slotUpdateMenu);
    readConfig();
}

ShowImageDialog::~ShowImageDialog()
{
    writeConfig();
}

void ShowImageDialog::slotUpdateMenu(const ShowImageWidget::ImageInfo &info)
{
    updateServiceList();
    if (info.isAnimatedImage) {
        mClipboardImageAction->setEnabled(false);
    }
}

void ShowImageDialog::setImageInfo(const ShowImageWidget::ImageInfo &info)
{
    mShowImageWidget->setImageInfo(info);
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

void ShowImageDialog::updateServiceList()
{
    mOpenWithMenu->clear();
    QMimeDatabase db;
    const QString path = mRocketChatAccount->attachmentUrlFromLocalCache(mShowImageWidget->imageInfo().bigImagePath).toLocalFile();
    if (path.isEmpty()) {
        return;
    }
    const QMimeType mimeType = db.mimeTypeForFile(path);

    const bool valid = mimeType.isValid() && !mimeType.isDefault();
    mServiceList = valid ? KApplicationTrader::queryByMimeType(mimeType.name()) : KService::List{};

    int idx = 0;
    for (const KService::Ptr &service : std::as_const(mServiceList)) {
        const QString text = service->name().replace(u'&', u"&&"_s);
        QAction *action = mOpenWithMenu->addAction(text);
        action->setIcon(QIcon::fromTheme(service->icon()));
        action->setData(idx);
        if (idx == 0) {
            mOpenWithButton->setDefaultAction(action);
        }
        ++idx;
    }
    mOpenWithMenu->addSeparator();
    QAction *action = mOpenWithMenu->addAction(QIcon::fromTheme(QStringLiteral("system-run")), i18n("Other Application..."));
    action->setData(-1);
}

void ShowImageDialog::slotOpenWith(QAction *action)
{
    bool ok;
    const int idx = action->data().toInt(&ok);
    if (!ok) {
        return;
    }
    KService::Ptr service;
    if (idx != -1) {
        service = mServiceList.at(idx);
    }
    mShowImageWidget->openWith(service);
}

#include "moc_showimagedialog.cpp"
