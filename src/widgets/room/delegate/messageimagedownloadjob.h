/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class RocketChatAccount;
class MessageImageDownloadJob : public QObject
{
    Q_OBJECT
public:
    struct MessageImageDownloadJobInfo {
        Q_REQUIRED_RESULT bool canStart() const;
        QString bigImagePath;
        bool needToDownloadBigImage = false;
        QWidget *parentWidget = nullptr;
    };

    explicit MessageImageDownloadJob(QObject *parent = nullptr);
    ~MessageImageDownloadJob() override;

    Q_REQUIRED_RESULT MessageImageDownloadJobInfo info() const;
    void setInfo(const MessageImageDownloadJobInfo &newInfo);

    Q_REQUIRED_RESULT RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    Q_REQUIRED_RESULT bool canStart() const;

    void start();

private:
    void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    MessageImageDownloadJobInfo mInfo;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
