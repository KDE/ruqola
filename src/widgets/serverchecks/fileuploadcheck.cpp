/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadcheck.h"

#include "messages/messageattachment.h"
#include "messages/messageattachments.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "uploadfilemanager.h"

#include <KLocalizedString>
#include <QDateTime>
#include <QDir>
#include <QTemporaryFile>
#include <QUrl>

using namespace Qt::Literals::StringLiterals;

QString FileUploadCheck::name() const
{
    return i18n("File attachment upload");
}

void FileUploadCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mFileName = u"ruqola-check-%1.txt"_s.arg(QDateTime::currentMSecsSinceEpoch());

    auto tempFile = new QTemporaryFile(QDir::tempPath() + u"/XXXXXX.txt"_s, this);
    tempFile->setAutoRemove(false);
    if (!tempFile->open()) {
        reportFailed(i18n("Cannot create a temporary file."));
        return;
    }
    tempFile->write("ruqola server checklist test file\n");
    tempFile->close();

    connectModel();

    RocketChatRestApi::UploadFileJob::UploadFileInfo info;
    info.roomId = mRoomId;
    info.filenameUrl = QUrl::fromLocalFile(tempFile->fileName());
    info.fileName = mFileName;
    info.deleteTemporaryFile = true;
    info.rc80Server = mAccount->hasAtLeastVersion(8, 0, 0);
    static_cast<void>(mAccount->uploadFileManager()->addUpload(info));
}

void FileUploadCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    auto model = messageModel();
    if (!model) {
        return;
    }
    const int rows = model->rowCount();
    for (int row = 0; row < rows; ++row) {
        const QByteArray messageId = model->messageIdFromIndex(row);
        const Message message = model->findMessageById(messageId);
        if (message.pendingMessage() || !message.attachments()) {
            continue;
        }
        const QList<MessageAttachment> attachments = message.attachments()->messageAttachments();
        for (const MessageAttachment &attachment : attachments) {
            if (attachment.title() != mFileName && !attachment.link().contains(mFileName)) {
                continue;
            }
            if (attachment.attachmentType() == MessageAttachment::AttachmentType::File) {
                reportPassed();
            } else {
                reportFailed(i18n("The file arrived but was not recognized as a file attachment."));
            }
            deleteTestMessage(messageId);
            return;
        }
    }
}

#include "moc_fileuploadcheck.cpp"
