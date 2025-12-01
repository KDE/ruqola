/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <TextAddonsWidgets/OpenFileJob>
class RocketChatAccount;
class MessageAttachmentDelegateHelperOpenFileJob : public TextAddonsWidgets::OpenFileJob
{
    Q_OBJECT
public:
    explicit MessageAttachmentDelegateHelperOpenFileJob(QObject *parent = nullptr);
    ~MessageAttachmentDelegateHelperOpenFileJob() override;

    void downloadFile(const QUrl &fileUrl) override;
    void runApplication(const KService::Ptr &offer) override;
    void openUrl() override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

private:
    RocketChatAccount *mRocketChatAccount = nullptr;
};
