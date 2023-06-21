/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigureavatarwidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImageReader>
#include <QInputDialog>
#include <QMenu>

MyAccountProfileConfigureAvatarWidget::MyAccountProfileConfigureAvatarWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mAvatarImage(new AvatarImage(account, this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mAvatarImage->setObjectName(QStringLiteral("mAvatarImage"));
    mainLayout->addWidget(mAvatarImage, 0, Qt::AlignLeft);
}

MyAccountProfileConfigureAvatarWidget::~MyAccountProfileConfigureAvatarWidget() = default;

void MyAccountProfileConfigureAvatarWidget::setCurrentIconPath(const QString &currentPath)
{
    mAvatarImage->setCurrentIconPath(currentPath);
}

AvatarImage::AvatarImage(RocketChatAccount *account, QWidget *parent)
    : QPushButton(parent)
    , mRocketChatAccount(account)
{
    setIconSize(QSize(100, 100));
    setFixedSize(QSize(120, 120));

    connect(this, &AvatarImage::clicked, this, &AvatarImage::changeImage);
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &AvatarImage::slotFileDownloaded);
    }
}

AvatarImage::~AvatarImage() = default;

void AvatarImage::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    Q_UNUSED(filePath)
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = mRocketChatAccount->ownUser().userName();
    const QUrl iconUrlStr = QUrl(mRocketChatAccount->avatarUrl(info));
    if (!iconUrlStr.isEmpty()) {
        if (iconUrlStr == cacheImageUrl) {
            setCurrentIconPath(cacheImageUrl.toLocalFile());
        }
    }
}

void AvatarImage::changeImage()
{
    QString filter;
    const QList<QByteArray> supportedImage = QImageReader::supportedImageFormats();
    for (const QByteArray &ba : supportedImage) {
        if (!filter.isEmpty()) {
            filter += QLatin1Char(' ');
        }
        filter += QLatin1String("*.") + QString::fromLatin1(ba);
    }
    filter = QStringLiteral("%1 (%2)").arg(i18n("Image"), filter);
    const QUrl url = QFileDialog::getOpenFileUrl(this, i18n("Select Image"), {}, filter);
    if (!url.isEmpty()) {
        mRocketChatAccount->setImageUrl(url);
        setCurrentIconPath(url.toLocalFile());
    }
}

void AvatarImage::changeUrl()
{
    bool ok = false;
    const QString url = QInputDialog::getText(this, i18n("Change Url"), i18n("Define Avatar Url:"), QLineEdit::Normal, {}, &ok);
    if (ok && !url.isEmpty()) {
        mRocketChatAccount->setAvatarUrl(url);
    }
}

void AvatarImage::resetAvatar()
{
    mRocketChatAccount->resetAvatar();
}

void AvatarImage::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(i18n("Change Picture..."), this, &AvatarImage::changeImage);
    menu.addAction(i18n("Change URL for avatar..."), this, &AvatarImage::changeUrl);
    menu.addSeparator();
    menu.addAction(i18n("Reset Avatar"), this, &AvatarImage::resetAvatar);
    menu.exec(event->globalPos());
}

void AvatarImage::setCurrentIconPath(const QString &currentPath)
{
    mCurrentIconPath = currentPath;
    setIcon(QIcon(mCurrentIconPath));
}

#include "moc_myaccountprofileconfigureavatarwidget.cpp"
