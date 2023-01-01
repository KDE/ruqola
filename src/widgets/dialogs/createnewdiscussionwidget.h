/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class ChannelSearchWidget;
class KTextEdit;
class AddUsersWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewDiscussionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewDiscussionWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateNewDiscussionWidget() override;

    void setChannelInfo(const QString &name, const QString &channelId);
    Q_REQUIRED_RESULT QString channelName() const;

    void setDiscussionName(const QString &name);
    Q_REQUIRED_RESULT QString discussionName() const;

    void setMessage(const QString &name);
    Q_REQUIRED_RESULT QString message() const;

    Q_REQUIRED_RESULT QStringList usersId() const;

    Q_REQUIRED_RESULT QString channelId() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    ChannelSearchWidget *const mChannelSearchWidget;
    QLineEdit *const mDiscussionNameLineEdit;
    AddUsersWidget *const mUsers;
    KTextEdit *const mMessageTextEdit;
};
