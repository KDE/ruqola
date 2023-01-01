/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ThreadMessageWidget;
class RocketChatAccount;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ThreadMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ThreadMessageDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ThreadMessageDialog() override;

    void setThreadMessageId(const QString &threadMessageId);
    Q_REQUIRED_RESULT QString threadMessageId() const;

    void setFollowingThread(bool threadIsFollowing);

    void setThreadPreview(const QString &preview);
    void setRoom(Room *room);

private:
    void writeConfig();
    void readConfig();
    ThreadMessageWidget *const mThreadMessageWidget;
};
