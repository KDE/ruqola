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
    struct MessageImageDownloadJobInfo {
        [[nodiscard]] bool canStart() const;
        QString bigImagePath;
        bool needToDownloadBigImage = false;
        QWidget *parentWidget = nullptr;
    };

    explicit MessageAttachmentDownloadJob(QObject *parent = nullptr);
    ~MessageAttachmentDownloadJob() override;

    [[nodiscard]] MessageImageDownloadJobInfo info() const;
    void setInfo(const MessageImageDownloadJobInfo &newInfo);

    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    [[nodiscard]] bool canStart() const;

    void start();

private:
    void slotDownloadCancel();
    void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    MessageImageDownloadJobInfo mInfo;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QProgressDialog *mProgressDialogBox = nullptr;
};
