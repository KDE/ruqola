/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilemanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"

UploadFileManager::UploadFileManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

UploadFileManager::~UploadFileManager()
{
}

void UploadFileManager::addUpload(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info)
{
    auto job = new RocketChatRestApi::UploadFileJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setUploadFileInfo(info);
    connect(job, &RocketChatRestApi::UploadFileJob::uploadProgress, this, [this](const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info) {
        // TODO
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start UploadFileJob job";
    }
}
