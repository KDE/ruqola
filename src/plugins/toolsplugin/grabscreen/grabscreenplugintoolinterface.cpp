/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenplugintoolinterface.h"

#include "grabscreenplugin_debug.h"
#include "grabscreenpluginjob.h"
#include "grabscreenplugintoolutil.h"

#include "rocketchataccount.h"
#include "ruqola.h"
#include "uploadfilejob.h"
#include "uploadfilemanager.h"

#include <QFile>
#include <QFileInfo>
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
            // Don't keep an empty file around, otherwise generateFileName() will skip over it forever.
            QFile::remove(imagePath);
            return;
        }
        QPointer<UploadFileDialog> dlg = new UploadFileDialog(mParentWidget);
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
    info.messageText = QString();
    info.filenameUrl = uploadFileInfo.fileUrl;
    info.roomId = mInfo.roomId;
    info.threadMessageId = mInfo.tmid;
    info.fileName = uploadFileInfo.fileName;
    info.deleteTemporaryFile = uploadFileInfo.deleteTemporaryFile;
    // Make sure that we don't switch account
    Ruqola::self()->setCurrentAccount(mInfo.accountName);
    auto account = Ruqola::self()->rocketChatAccount();
    if (!account) {
        qCWarning(RUQOLA_GRABSCREEN_PLUGIN_LOG) << "Impossible to find account" << mInfo.accountName;
        return;
    }
    info.rc80Server = account->hasAtLeastVersion(8, 0, 0);

    const int identifier = account->uploadFileManager()->addUpload(info);
    if (identifier != -1) {
        // TODO mUploadFileProgressStatusListWidget->addProgressStatusWidget(identifier);
    }
}

#include "moc_grabscreenplugintoolinterface.cpp"
