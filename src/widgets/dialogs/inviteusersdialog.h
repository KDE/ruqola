/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);
    void generateLink();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    InviteUsersWidget *const mInviteUsersWidget;
};
