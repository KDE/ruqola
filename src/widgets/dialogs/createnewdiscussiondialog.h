/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
        QList<QByteArray> users;
        QString discussionName;
        QString channelName;
        QString message;
        QByteArray channelId;
    };
    explicit CreateNewDiscussionDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateNewDiscussionDialog() override;

    [[nodiscard]] NewDiscussionInfo newDiscussionInfo() const;

    void setChannelInfo(const QString &name, const QByteArray &channelId);
    void setDiscussionName(const QString &name);

    [[nodiscard]] const QByteArray &messageId() const;
    void setMessageId(const QByteArray &newMessageId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    QByteArray mMessageId;
    CreateNewDiscussionWidget *const mCreateNewDiscussionWidget;
    RocketChatAccount *const mCurrentRocketChatAccount;
};
