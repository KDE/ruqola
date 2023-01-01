/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilemanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"

#include <QFile>

int UploadFileManager::uploadIdentifier = 0;
UploadFileManager::UploadFileManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

UploadFileManager::~UploadFileManager() = default;

int UploadFileManager::addUpload(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info)
{
    QPointer<RocketChatRestApi::UploadFileJob> job = new RocketChatRestApi::UploadFileJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setUploadFileInfo(info);
    uploadIdentifier++;
    const int jobIdentifier = uploadIdentifier;
    connect(job,
            &RocketChatRestApi::UploadFileJob::uploadProgress,
            this,
            [this, jobIdentifier](const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info) {
                Q_EMIT uploadProgress(info, jobIdentifier, mRocketChatAccount->accountName());
            });
    // Need to delete temporary file.
    if (info.deleteTemporaryFile) {
        connect(job, &RocketChatRestApi::UploadFileJob::uploadFinished, this, [info]() {
            QFile f(info.filenameUrl.toLocalFile());
            if (f.remove()) {
                qCWarning(RUQOLA_LOG) << "Impossible to delete file" << f.fileName();
            }
        });
    }
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start UploadFileJob job";
        return -1;
    } else {
        mUploadMap.insert(uploadIdentifier, job);
        return uploadIdentifier;
    }
}

void UploadFileManager::removeFile(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info)
{
    if (info.deleteTemporaryFile) {
        QFile f(info.filenameUrl.toLocalFile());
        if (f.remove()) {
            qCWarning(RUQOLA_LOG) << "Impossible to delete file" << f.fileName();
        }
    }
}

void UploadFileManager::cancelJob(int identifier)
{
    auto job = mUploadMap.take(identifier);
    // Abort will remove job too.
    if (job) {
        removeFile(job->uploadFileInfo());
        job->abort();
    }
}
