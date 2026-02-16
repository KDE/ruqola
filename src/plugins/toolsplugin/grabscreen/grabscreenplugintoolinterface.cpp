/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenplugintoolinterface.h"

#include "accountmanager.h"
#include "grabscreenplugin_debug.h"
#include "grabscreenpluginjob.h"
#include "grabscreenplugintoolutil.h"

#include "rocketchataccount.h"
#include "ruqola.h"
#include "uploadfilejob.h"
#include "uploadfilemanager.h"

#include <QFileInfo>
#include <QPixmap>
#include <QPointer>
#include <QUrl>

GrabScreenPluginToolInterface::GrabScreenPluginToolInterface(QWidget *parentWidget, QObject *parent)
    : PluginToolInterface{parentWidget, parent}
{
}

GrabScreenPluginToolInterface::~GrabScreenPluginToolInterface() = default;

void GrabScreenPluginToolInterface::activateTool()
{
    QString imagePath = GrabScreenPluginToolUtil::picturePath(mInfo.accountName);
    if (imagePath.isEmpty()) {
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << " Impossible to create directory";
        return;
    }
    imagePath = GrabScreenPluginToolUtil::generateFileName(imagePath);

    auto job = new GrabScreenPluginJob(this);
    job->setFilePath(imagePath);
    connect(job, &GrabScreenPluginJob::captureCanceled, this, []() {
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << " CANCELED!!!!!";
    });
    connect(job, &GrabScreenPluginJob::captureDone, this, [this, imagePath]() {
        const QFileInfo fi(imagePath);
        if (fi.size() == 0) { // Canceled
            return;
        }
        QPointer<UploadFileDialog> dlg = new UploadFileDialog();
        dlg->setFileUrl(QUrl::fromLocalFile(imagePath));
        if (dlg->exec()) {
            const UploadFileDialog::UploadFileInfo uploadFileInfo = dlg->fileInfo();
            sendFile(uploadFileInfo);
        }
        delete dlg;
        Q_EMIT executed();
    });
    job->start();
}

void GrabScreenPluginToolInterface::sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo)
{
    RocketChatRestApi::UploadFileJob::UploadFileInfo info;
    info.description = uploadFileInfo.description;
    info.messageText = QString();
    info.filenameUrl = uploadFileInfo.fileUrl;
    info.roomId = mInfo.roomId;
    info.threadMessageId = mInfo.tmid;
    info.fileName = uploadFileInfo.fileName;
    info.deleteTemporaryFile = uploadFileInfo.deleteTemporaryFile;
    info.rc80Server = Ruqola::self()->rocketChatAccount()->hasAtLeastVersion(8, 0, 0);
    // Make sure that we don't switch account
    Ruqola::self()->setCurrentAccount(mInfo.accountName);

    const int identifier = Ruqola::self()->rocketChatAccount()->uploadFileManager()->addUpload(info);
    if (identifier != -1) {
        // TODO mUploadFileProgressStatusListWidget->addProgressStatusWidget(identifier);
    }

    // Q_EMIT createUploadJob(std::move(info));
}
