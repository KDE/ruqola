/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class InviteUsersWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT InviteUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InviteUsersDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~InviteUsersDialog() override;
    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);
    void generateLink();

private:
    void readConfig();
    void writeConfig();
    InviteUsersWidget *const mInviteUsersWidget;
};
