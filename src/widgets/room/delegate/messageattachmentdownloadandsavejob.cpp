/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdownloadandsavejob.h"
#include "common/delegateutil.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QProgressDialog>

MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadAndSaveJob(QObject *parent)
    : QObject{parent}
{
    connect(this, &MessageAttachmentDownloadAndSaveJob::downloadDone, this, &MessageAttachmentDownloadAndSaveJob::slotDownloadDone);
}

MessageAttachmentDownloadAndSaveJob::~MessageAttachmentDownloadAndSaveJob() = default;

void MessageAttachmentDownloadAndSaveJob::slotDownloadDone(const QString &path)
{
    switch (mInfo.actionType) {
    case MessageAttachmentDownloadAndSaveJob::ActionType::DownloadAndSave:
        DelegateUtil::saveFile(mInfo.parentWidget, path, saveFileString());
        break;
    case MessageAttachmentDownloadAndSaveJob::ActionType::DownloadOnly:
        Q_EMIT attachmentFileDownloadDone(path);
        break;
    case MessageAttachmentDownloadAndSaveJob::ActionType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "ActionType is unknown. It's a bug";
        break;
    }
    slotDownloadCancel();
}

bool MessageAttachmentDownloadAndSaveJob::canStart() const
{
    return mInfo.canStart();
}

void MessageAttachmentDownloadAndSaveJob::slotFileDownloaded(const QString &filePath, const QUrl &cacheAttachmentUrl)
{
    qCDebug(RUQOLAWIDGETS_LOG) << "File Downloaded : " << filePath << " cacheImageUrl " << cacheAttachmentUrl;
    if (filePath == QUrl(mInfo.attachmentPath).toString()) {
        const QString cacheAttachmentUrlPath{cacheAttachmentUrl.toLocalFile()};
        Q_EMIT downloadDone(cacheAttachmentUrlPath);
    }
}

QString MessageAttachmentDownloadAndSaveJob::saveFileString() const
{
    QString str;
    switch (mInfo.attachmentType) {
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Unknown:
        break;
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Image:
        str = i18n("Save Image");
        break;
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Video:
        str = i18n("Save Video");
        break;
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Sound:
        str = i18n("Save Sound");
        break;
    }
    return str;
}

void MessageAttachmentDownloadAndSaveJob::assignProgressDialogStr(QProgressDialog *progressDialog)
{
    switch (mInfo.attachmentType) {
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Unknown:
        break;
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Image:
        progressDialog->setWindowTitle(i18nc("@title:window", "Download Image"));
        progressDialog->setLabelText(i18n("Download Image..."));
        break;
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Video:
        progressDialog->setWindowTitle(i18nc("@title:window", "Download Video"));
        progressDialog->setLabelText(i18n("Download Video..."));
        break;
    case MessageAttachmentDownloadAndSaveJob::AttachmentType::Sound:
        progressDialog->setWindowTitle(i18nc("@title:window", "Download Sound"));
        progressDialog->setLabelText(i18n("Download Sound..."));
        break;
    }
}

void MessageAttachmentDownloadAndSaveJob::slotDownloadCancel()
{
    if (mProgressDialogBox) {
        mProgressDialogBox->hide();
        mProgressDialogBox->deleteLater();
    }
    deleteLater();
}

void MessageAttachmentDownloadAndSaveJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Attachment url empty";
        deleteLater();
        return;
    }
    if (!mRocketChatAccount) {
        qCWarning(RUQOLAWIDGETS_LOG) << "mRocketChatAccount is empty. It's a bug";
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
            connect(mProgressDialogBox, &QProgressDialog::canceled, this, &MessageAttachmentDownloadAndSaveJob::slotDownloadCancel);
            connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessageAttachmentDownloadAndSaveJob::slotFileDownloaded);
            (void)mRocketChatAccount->attachmentUrlFromLocalCache(mInfo.attachmentPath);
        }
    } else {
        Q_EMIT downloadDone(mRocketChatAccount->attachmentUrlFromLocalCache(mInfo.attachmentPath).toLocalFile());
    }
}

RocketChatAccount *MessageAttachmentDownloadAndSaveJob::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void MessageAttachmentDownloadAndSaveJob::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo MessageAttachmentDownloadAndSaveJob::info() const
{
    return mInfo;
}

void MessageAttachmentDownloadAndSaveJob::setInfo(const MessageAttachmentDownloadJobInfo &newInfo)
{
    mInfo = newInfo;
}

bool MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo::canStart() const
{
    if (attachmentType == MessageAttachmentDownloadAndSaveJob::AttachmentType::Unknown) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Attachment type not defined";
        return false;
    }
    return !attachmentPath.isEmpty();
}

#include "moc_messageattachmentdownloadandsavejob.cpp"
