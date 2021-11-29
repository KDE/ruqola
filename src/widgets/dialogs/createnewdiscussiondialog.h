/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class CreateNewDiscussionWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewDiscussionDialog : public QDialog
{
    Q_OBJECT
public:
    struct NewDiscussionInfo {
        QStringList users;
        QString discussionName;
        QString channelName;
        QString message;
        QString channelId;
    };
    explicit CreateNewDiscussionDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateNewDiscussionDialog() override;

    Q_REQUIRED_RESULT NewDiscussionInfo newDiscussionInfo() const;

    void setChannelInfo(const QString &name, const QString &channelId);
    void setDiscussionName(const QString &name);

    const QString &messageId() const;
    void setMessageId(const QString &newMessageId);

private:
    void readConfig();
    void writeConfig();
    void createNewDiscussion();
    QString mMessageId;
    CreateNewDiscussionWidget *const mCreateNewDiscussionWidget;
    RocketChatAccount *const mCurrentRocketChatAccount;
};

