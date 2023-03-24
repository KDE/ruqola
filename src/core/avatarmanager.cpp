/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "avatarmanager.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include <QTimer>
#include <chrono>
using namespace std::chrono_literals;

AvatarManager::AvatarManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
    , mTimer(new QTimer(this))
{
    mTimer->setSingleShot(true);
    // increase interval otherwise we can have some error
    mTimer->setInterval(2s);
    connect(mTimer, &QTimer::timeout, this, &AvatarManager::slotLoadNextAvatar);
}

AvatarManager::~AvatarManager() = default;

void AvatarManager::slotLoadNextAvatar()
{
    const Utils::AvatarInfo info = mAvatarDownloadIdentifer.constFirst();
    const QUrl url = Utils::avatarUrl(mAccount->serverUrl(), info);
    // qDebug() << " url " << url;
    if (url.isEmpty()) {
        return;
    }
    slotInsertAvatarUrl(info, url);
}

void AvatarManager::slotRescheduleDownload()
{
    // if problem we need to reschedule after several seconds
    QTimer::singleShot(20s, this, &AvatarManager::slotLoadNextAvatar);
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

void AvatarManager::slotInsertAvatarUrl(const Utils::AvatarInfo &info, const QUrl &url)
{
    const QString identifier = info.generateAvatarIdentifier();
    qDebug() << "AvatarManager::slotInsertAvatarUrl: identifier " << identifier;
    // Use etag in identifier ?
    if (!url.isEmpty()) {
        Q_EMIT insertAvatarUrl(identifier, url);
    } // Else error for downloading => don't redownload it + continue.

    mAvatarDownloadIdentifer.removeAll(info);
    // qDebug() << " mAvatarDownloadUserIds" << mAvatarDownloadUserIds;
    if (!mAvatarDownloadIdentifer.isEmpty()) {
        mTimer->start();
    }
}
