/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdownloadjob.h"
#include "common/delegateutil.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QProgressDialog>

MessageAttachmentDownloadJob::MessageAttachmentDownloadJob(QObject *parent)
    : QObject{parent}
{
}

MessageAttachmentDownloadJob::~MessageAttachmentDownloadJob() = default;

bool MessageAttachmentDownloadJob::canStart() const
{
    return mInfo.canStart();
}

void MessageAttachmentDownloadJob::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    qCDebug(RUQOLAWIDGETS_LOG) << "File Downloaded : " << filePath << " cacheImageUrl " << cacheImageUrl;
    if (filePath == QUrl(mInfo.bigImagePath).toString()) {
        const QString cacheImageUrlPath{cacheImageUrl.toLocalFile()};
        DelegateUtil::saveFile(mInfo.parentWidget, cacheImageUrlPath, i18n("Save Image"));
        slotDownloadCancel();
    }
}

void MessageAttachmentDownloadJob::slotDownloadCancel()
{
    if (mProgressDialogBox) {
        mProgressDialogBox->hide();
        mProgressDialogBox->deleteLater();
    }
    mProgressDialogBox = nullptr;
    deleteLater();
}

void MessageAttachmentDownloadJob::start()
{
    if (!canStart()) {
        deleteLater();
        return;
    }

    if (mInfo.needToDownloadBigImage) {
        if (mRocketChatAccount) {
            mProgressDialogBox = new QProgressDialog(mInfo.parentWidget);
            mProgressDialogBox->setWindowTitle(i18nc("@title:window", "Download Image"));
            mProgressDialogBox->setLabelText(i18n("Download Image..."));
            mProgressDialogBox->reset();
            mProgressDialogBox->setRange(0, 0);
            mProgressDialogBox->setValue(0);
            mProgressDialogBox->setModal(true);
            mProgressDialogBox->setAutoClose(false);
            mProgressDialogBox->setAutoReset(false);
            mProgressDialogBox->setMinimumDuration(0);
            connect(mProgressDialogBox, &QProgressDialog::canceled, this, &MessageAttachmentDownloadJob::slotDownloadCancel);
            connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessageAttachmentDownloadJob::slotFileDownloaded);
            (void)mRocketChatAccount->attachmentUrlFromLocalCache(mInfo.bigImagePath);
        }
    } else {
        DelegateUtil::saveFile(mInfo.parentWidget, mRocketChatAccount->attachmentUrlFromLocalCache(mInfo.bigImagePath).toLocalFile(), i18n("Save Image"));
        deleteLater();
    }
}

RocketChatAccount *MessageAttachmentDownloadJob::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void MessageAttachmentDownloadJob::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

MessageAttachmentDownloadJob::MessageImageDownloadJobInfo MessageAttachmentDownloadJob::info() const
{
    return mInfo;
}

void MessageAttachmentDownloadJob::setInfo(const MessageImageDownloadJobInfo &newInfo)
{
    mInfo = newInfo;
}

bool MessageAttachmentDownloadJob::MessageImageDownloadJobInfo::canStart() const
{
    return !bigImagePath.isEmpty();
}

#include "moc_messageattachmentdownloadjob.cpp"
