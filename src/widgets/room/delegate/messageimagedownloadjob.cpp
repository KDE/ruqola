/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageimagedownloadjob.h"
#include "common/delegateutil.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>

MessageImageDownloadJob::MessageImageDownloadJob(QObject *parent)
    : QObject{parent}
{
}

MessageImageDownloadJob::~MessageImageDownloadJob() = default;

bool MessageImageDownloadJob::canStart() const
{
    return mInfo.canStart();
}

void MessageImageDownloadJob::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    qCDebug(RUQOLAWIDGETS_LOG) << "File Downloaded : " << filePath << " cacheImageUrl " << cacheImageUrl;
    if (filePath == QUrl(mInfo.bigImagePath).toString()) {
        const QString cacheImageUrlPath{cacheImageUrl.toLocalFile()};
        DelegateUtil::saveFile(mInfo.parentWidget, cacheImageUrlPath, i18n("Save Image"));
        deleteLater();
    }
}

void MessageImageDownloadJob::start()
{
    if (!canStart()) {
        deleteLater();
        return;
    }

    if (mInfo.needToDownloadBigImage) {
        if (mRocketChatAccount) {
            connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessageImageDownloadJob::slotFileDownloaded);
            (void)mRocketChatAccount->attachmentUrlFromLocalCache(mInfo.bigImagePath);
        }
    } else {
        DelegateUtil::saveFile(mInfo.parentWidget, mInfo.bigImagePath, i18n("Save Image"));
        deleteLater();
    }
}

RocketChatAccount *MessageImageDownloadJob::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void MessageImageDownloadJob::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

MessageImageDownloadJob::MessageImageDownloadJobInfo MessageImageDownloadJob::info() const
{
    return mInfo;
}

void MessageImageDownloadJob::setInfo(const MessageImageDownloadJobInfo &newInfo)
{
    mInfo = newInfo;
}

bool MessageImageDownloadJob::MessageImageDownloadJobInfo::canStart() const
{
    return !bigImagePath.isEmpty();
}
