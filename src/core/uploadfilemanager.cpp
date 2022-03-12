/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilemanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"

int UploadFileManager::uploadIdentifier = 0;
UploadFileManager::UploadFileManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

UploadFileManager::~UploadFileManager()
{
}

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
                Q_EMIT uploadProgress(info, jobIdentifier);
            });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start UploadFileJob job";
        return -1;
    } else {
        mUploadMap.insert(uploadIdentifier, job);
        return uploadIdentifier;
    }
}

void UploadFileManager::cancelJob(int identifier)
{
    auto job = mUploadMap.take(identifier);
    // Abort will remove job too.
    if (job) {
        job->abort();
    }
}
