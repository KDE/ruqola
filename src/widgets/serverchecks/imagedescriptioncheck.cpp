/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "imagedescriptioncheck.h"

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

namespace
{
// A 1x1 transparent PNG.
const char pngBase64[] = "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mNk+M8AAAMBAQDJ/pLvAAAAAElFTkSuQmCC";
}

QString ImageDescriptionCheck::name() const
{
    return i18n("Image description");
}

void ImageDescriptionCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    const qint64 stamp = QDateTime::currentMSecsSinceEpoch();
    mDescription = u"ruqola-check-description-%1"_s.arg(stamp);
    mFileName = u"ruqola-check-%1.png"_s.arg(stamp);

    auto tempFile = new QTemporaryFile(QDir::tempPath() + u"/XXXXXX.png"_s, this);
    tempFile->setAutoRemove(false);
    if (!tempFile->open()) {
        reportFailed(i18n("Cannot create a temporary image file."));
        return;
    }
    tempFile->write(QByteArray::fromBase64(pngBase64));
    tempFile->close();

    connectModel();

    RocketChatRestApi::UploadFileJob::UploadFileInfo info;
    info.roomId = mRoomId;
    info.description = mDescription;
    info.messageText = QString();
    info.filenameUrl = QUrl::fromLocalFile(tempFile->fileName());
    info.fileName = mFileName;
    info.deleteTemporaryFile = true;
    info.rc80Server = mAccount->hasAtLeastVersion(8, 0, 0);
    static_cast<void>(mAccount->uploadFileManager()->addUpload(info));
}

void ImageDescriptionCheck::checkState()
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
            const bool matchesOurUpload = attachment.title() == mFileName || attachment.link().contains(mFileName);
            if (!matchesOurUpload) {
                continue;
            }
            if (attachment.description() == mDescription) {
                reportPassed();
            } else if (attachment.description().isEmpty()) {
                reportFailed(i18n("The image arrived but its description was dropped by the server."));
            } else {
                reportFailed(i18n("The image arrived with a wrong description: \"%1\".", attachment.description()));
            }
            deleteTestMessage(messageId);
            return;
        }
    }
}

#include "moc_imagedescriptioncheck.cpp"
