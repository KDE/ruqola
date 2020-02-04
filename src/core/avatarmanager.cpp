/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "avatarmanager.h"
#include "rocketchataccount.h"
#include "restapirequest.h"
#include "ruqola_debug.h"
#include <QTimer>

AvatarManager::AvatarManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    //increase interval otherwise we can have some error
    mTimer->setInterval(2000);
    connect(mTimer, &QTimer::timeout, this, &AvatarManager::slotLoadNextAvatar);
    connect(mAccount->restApi(), &RocketChatRestApi::RestApiRequest::avatar, this, &AvatarManager::slotInsertAvatarUrl);
    connect(mAccount->restApi(), &RocketChatRestApi::RestApiRequest::redownloadAvatar, this, &AvatarManager::slotRescheduleDownload);
}

AvatarManager::~AvatarManager()
{
}

void AvatarManager::slotLoadNextAvatar()
{
    mAccount->restApi()->getAvatar(mAvatarDownloadUserIds.first());
}

void AvatarManager::slotRescheduleDownload()
{
    //if problem we need to reschedule after several seconds
    QTimer::singleShot(20000, this, &AvatarManager::slotLoadNextAvatar);
}

void AvatarManager::insertInDownloadQueue(const QString &userId)
{
    if (userId.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "AvatarManager::insertInDownloadQueue userid is empty!";
        return;
    }
    bool startDownload = false;
    if (mAvatarDownloadUserIds.isEmpty()) {
        startDownload = true;
    }
    if (!mAvatarDownloadUserIds.contains(userId)) {
        mAvatarDownloadUserIds.append(userId);
    }
    if (startDownload) {
        mTimer->start();
    }
}

RocketChatAccount *AvatarManager::account() const
{
    return mAccount;
}

void AvatarManager::slotInsertAvatarUrl(const QString &userId, const QString &url)
{
    Q_EMIT insertAvatarUrl(userId, QUrl(url));
    mAvatarDownloadUserIds.removeAll(userId);
    //qDebug() << " mAvatarDownloadUserIds" << mAvatarDownloadUserIds;
    if (!mAvatarDownloadUserIds.isEmpty()) {
        mTimer->start();
    }
}
