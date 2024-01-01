/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "uploadfilejob.h"
#include <QMap>
#include <QObject>
#include <QPointer>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT UploadFileManager : public QObject
{
    Q_OBJECT
public:
    explicit UploadFileManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~UploadFileManager() override;

    [[nodiscard]] int addUpload(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info);

    void cancelJob(int identifier);
Q_SIGNALS:
    void uploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info, int identifier, const QString &accountName);

private:
    LIBRUQOLACORE_NO_EXPORT void removeFile(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info);
    RocketChatAccount *const mRocketChatAccount;
    QMap<int, QPointer<RocketChatRestApi::UploadFileJob>> mUploadMap;
    static int uploadIdentifier;
};
