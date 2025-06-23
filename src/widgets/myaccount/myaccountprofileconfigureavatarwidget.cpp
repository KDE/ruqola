/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigureavatarwidget.h"
#include "connection.h"
#include "ruqolawidgets_debug.h"
#include "users/resetavatarjob.h"
#include "users/userbasejob.h"

#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImageReader>
#include <QInputDialog>
#include <QMenu>

using namespace Qt::Literals::StringLiterals;
MyAccountProfileConfigureAvatarWidget::MyAccountProfileConfigureAvatarWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mAvatarImage(new AvatarImage(account, this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);
    mAvatarImage->setObjectName(u"mAvatarImage"_s);
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
            filter += u' ';
        }
        filter += "*."_L1 + QString::fromLatin1(ba);
    }
    filter = u"%1 (%2)"_s.arg(i18n("Image"), filter);
    const QUrl url = QFileDialog::getOpenFileUrl(this, i18nc("@title:window", "Select Image"), {}, filter);
    if (!url.isEmpty()) {
        mRocketChatAccount->setImageUrl(url);
        setCurrentIconPath(url.toLocalFile());
    }
}

void AvatarImage::changeUrl()
{
    bool ok = false;
    const QString url = QInputDialog::getText(this, i18nc("@title:window", "Change Url"), i18n("Define Avatar Url:"), QLineEdit::Normal, {}, &ok);
    if (ok && !url.isEmpty()) {
        mRocketChatAccount->setAvatarUrl(url);
    }
}

void AvatarImage::resetAvatar()
{
    RocketChatRestApi::UserBaseJob::UserInfo info;
    info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    info.userIdentifier = QString::fromLatin1(mRocketChatAccount->userId());
    auto job = new RocketChatRestApi::ResetAvatarJob(this);
    job->setUserInfo(info);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ResetAvatarJob";
    }
}

void AvatarImage::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(i18nc("@action", "Change Picture…"), this, &AvatarImage::changeImage);
    menu.addAction(i18nc("@action", "Change URL for avatar…"), this, &AvatarImage::changeUrl);
    menu.addSeparator();
    menu.addAction(i18nc("@action", "Reset Avatar"), this, &AvatarImage::resetAvatar);
    menu.exec(event->globalPos());
}

void AvatarImage::setCurrentIconPath(const QString &currentPath)
{
    mCurrentIconPath = currentPath;
    setIcon(QIcon(mCurrentIconPath));
}

#include "moc_myaccountprofileconfigureavatarwidget.cpp"
