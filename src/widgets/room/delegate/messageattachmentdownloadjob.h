/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class RocketChatAccount;
class QProgressDialog;
class MessageAttachmentDownloadJob : public QObject
{
    Q_OBJECT
public:
    enum AttachmentType {
        Unknown = 0,
        Image,
        Video,
        Sound,
    };

    struct MessageAttachmentDownloadJobInfo {
        [[nodiscard]] bool canStart() const;
        QString attachmentPath;
        MessageAttachmentDownloadJob::AttachmentType type = MessageAttachmentDownloadJob::AttachmentType::Unknown;
        bool needToDownloadAttachment = false;
        QWidget *parentWidget = nullptr;
    };

    explicit MessageAttachmentDownloadJob(QObject *parent = nullptr);
    ~MessageAttachmentDownloadJob() override;

    [[nodiscard]] MessageAttachmentDownloadJobInfo info() const;
    void setInfo(const MessageAttachmentDownloadJobInfo &newInfo);

    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    [[nodiscard]] bool canStart() const;

    void start();

private:
    void slotDownloadCancel();
    void assignProgressDialogStr(QProgressDialog *progressDialog);
    void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    [[nodiscard]] QString saveFileString() const;
    MessageAttachmentDownloadJobInfo mInfo;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QProgressDialog *mProgressDialogBox = nullptr;
};
