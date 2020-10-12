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
}

AvatarManager::~AvatarManager()
{
}

void AvatarManager::slotLoadNextAvatar()
{
    const Utils::AvatarInfo info = mAvatarDownloadIdentifer.constFirst();
    const QUrl url = Utils::avatarUrl(mAccount->serverUrl(), info);
    qDebug() << " url " << url;
    if (url.isEmpty()) {
        return;
    }
    slotInsertAvatarUrl(info, url);
}

void AvatarManager::slotRescheduleDownload()
{
    //if problem we need to reschedule after several seconds
    QTimer::singleShot(20000, this, &AvatarManager::slotLoadNextAvatar);
}

void AvatarManager::insertInDownloadQueue(const Utils::AvatarInfo &info)
{
    if (!info.isValid()) {
        qCWarning(RUQOLA_LOG) << "AvatarManager::insertInDownloadQueue info is not valid!" << info;
        return;
    }
    bool startDownload = false;
    if (mAvatarDownloadIdentifer.isEmpty()) {
        startDownload = true;
    }
    if (!mAvatarDownloadIdentifer.contains(info)) {
        mAvatarDownloadIdentifer.append(info);
    }
    if (startDownload) {
        mTimer->start();
    }
}

RocketChatAccount *AvatarManager::account() const
{
    return mAccount;
}

void AvatarManager::slotInsertAvatarUrl(const Utils::AvatarInfo info, const QUrl &url)
{
    const QString identifier = info.identifier;
    if (!url.isEmpty()) {
        Q_EMIT insertAvatarUrl(identifier, url);
    } //Else error for downloading => don't redownload it + continue.

    mAvatarDownloadIdentifer.removeAll(info);
    //qDebug() << " mAvatarDownloadUserIds" << mAvatarDownloadUserIds;
    if (!mAvatarDownloadIdentifer.isEmpty()) {
        mTimer->start();
    }
}
