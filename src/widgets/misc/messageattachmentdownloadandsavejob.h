/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QObject>
class RocketChatAccount;
class QProgressDialog;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDownloadAndSaveJob : public QObject
{
    Q_OBJECT
public:
    enum class ActionType {
        Unknown = 0,
        DownloadOnly,
        DownloadAndSave,
    };
    Q_ENUM(ActionType)

    enum class AttachmentType {
        Unknown = 0,
        Image,
        Video,
        Sound,
    };
    Q_ENUM(AttachmentType)

    struct LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDownloadJobInfo {
        [[nodiscard]] bool isValid() const;
        QString attachmentPath;
        MessageAttachmentDownloadAndSaveJob::AttachmentType attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Unknown;
        MessageAttachmentDownloadAndSaveJob::ActionType actionType = MessageAttachmentDownloadAndSaveJob::ActionType::Unknown;
        bool needToDownloadAttachment = false;
        QWidget *parentWidget = nullptr;
    };

    explicit MessageAttachmentDownloadAndSaveJob(QObject *parent = nullptr);
    ~MessageAttachmentDownloadAndSaveJob() override;

    [[nodiscard]] MessageAttachmentDownloadJobInfo info() const;
    void setInfo(const MessageAttachmentDownloadJobInfo &newInfo);

    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    [[nodiscard]] bool canStart() const;

    void start();

Q_SIGNALS:
    void downloadDone(const QString &url);
    void attachmentFileDownloadDone(const QString &url);

private:
    void slotDownloadDone(const QString &path);
    void slotDownloadCancel();
    void assignProgressDialogStr(QProgressDialog *progressDialog);
    void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    [[nodiscard]] QString saveFileString() const;
    MessageAttachmentDownloadJobInfo mInfo;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QProgressDialog *mProgressDialogBox = nullptr;
};
