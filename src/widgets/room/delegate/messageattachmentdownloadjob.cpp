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

void MessageAttachmentDownloadJob::slotFileDownloaded(const QString &filePath, const QUrl &cacheAttachmentUrl)
{
    qCDebug(RUQOLAWIDGETS_LOG) << "File Downloaded : " << filePath << " cacheImageUrl " << cacheAttachmentUrl;
    if (filePath == QUrl(mInfo.attachmentPath).toString()) {
        const QString cacheAttachmentUrlPath{cacheAttachmentUrl.toLocalFile()};
        DelegateUtil::saveFile(mInfo.parentWidget, cacheAttachmentUrlPath, saveFileString());
        slotDownloadCancel();
    }
}

QString MessageAttachmentDownloadJob::saveFileString() const
{
    QString str;
    switch (mInfo.type) {
    case MessageAttachmentDownloadJob::AttachmentType::Unknown:
        break;
    case MessageAttachmentDownloadJob::AttachmentType::Image:
        str = i18n("Save Image");
        break;
    case MessageAttachmentDownloadJob::AttachmentType::Video:
        str = i18n("Save Video");
        break;
    case MessageAttachmentDownloadJob::AttachmentType::Sound:
        str = i18n("Save Sound");
        break;
    }
    return str;
}

void MessageAttachmentDownloadJob::assignProgressDialogStr(QProgressDialog *progressDialog)
{
    switch (mInfo.type) {
    case MessageAttachmentDownloadJob::AttachmentType::Unknown:
        break;
    case MessageAttachmentDownloadJob::AttachmentType::Image:
        progressDialog->setWindowTitle(i18nc("@title:window", "Download Image"));
        progressDialog->setLabelText(i18n("Download Image..."));
        break;
    case MessageAttachmentDownloadJob::AttachmentType::Video:
        progressDialog->setWindowTitle(i18nc("@title:window", "Download Video"));
        progressDialog->setLabelText(i18n("Download Video..."));
        break;
    case MessageAttachmentDownloadJob::AttachmentType::Sound:
        progressDialog->setWindowTitle(i18nc("@title:window", "Download Sound"));
        progressDialog->setLabelText(i18n("Download Sound..."));
        break;
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
        qCWarning(RUQOLAWIDGETS_LOG) << "Attachment url empty";
        deleteLater();
        return;
    }

    if (mInfo.needToDownloadAttachment) {
        if (mRocketChatAccount) {
            mProgressDialogBox = new QProgressDialog(mInfo.parentWidget);
            assignProgressDialogStr(mProgressDialogBox);
            mProgressDialogBox->reset();
            mProgressDialogBox->setRange(0, 0);
            mProgressDialogBox->setValue(0);
            mProgressDialogBox->setModal(true);
            mProgressDialogBox->setAutoClose(false);
            mProgressDialogBox->setAutoReset(false);
            mProgressDialogBox->setMinimumDuration(0);
            connect(mProgressDialogBox, &QProgressDialog::canceled, this, &MessageAttachmentDownloadJob::slotDownloadCancel);
            connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessageAttachmentDownloadJob::slotFileDownloaded);
            (void)mRocketChatAccount->attachmentUrlFromLocalCache(mInfo.attachmentPath);
        }
    } else {
        DelegateUtil::saveFile(mInfo.parentWidget, mRocketChatAccount->attachmentUrlFromLocalCache(mInfo.attachmentPath).toLocalFile(), saveFileString());
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

MessageAttachmentDownloadJob::MessageAttachmentDownloadJobInfo MessageAttachmentDownloadJob::info() const
{
    return mInfo;
}

void MessageAttachmentDownloadJob::setInfo(const MessageAttachmentDownloadJobInfo &newInfo)
{
    mInfo = newInfo;
}

bool MessageAttachmentDownloadJob::MessageAttachmentDownloadJobInfo::canStart() const
{
    if (type == MessageAttachmentDownloadJob::AttachmentType::Unknown) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Attachment type not defined";
        return false;
    }
    return !attachmentPath.isEmpty();
}

#include "moc_messageattachmentdownloadjob.cpp"
