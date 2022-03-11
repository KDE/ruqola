/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "uploadfilejob.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT UploadFileManager : public QObject
{
    Q_OBJECT
public:
    explicit UploadFileManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~UploadFileManager() override;

    void addUpload(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info);

private:
    RocketChatAccount *const mRocketChatAccount;
};
