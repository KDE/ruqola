/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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

    void setChannelInfo(const QString &name, const QByteArray &channelId);
    [[nodiscard]] QString channelName() const;

    void setDiscussionName(const QString &name);
    [[nodiscard]] QString discussionName() const;

    void setMessage(const QString &name);
    [[nodiscard]] QString message() const;

    [[nodiscard]] QList<QByteArray> usersId() const;

    [[nodiscard]] QByteArray channelId() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    ChannelSearchWidget *const mChannelSearchWidget;
    QLineEdit *const mDiscussionNameLineEdit;
    AddUsersWidget *const mUsers;
    KTextEdit *const mMessageTextEdit;
};
