/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class CreateDirectMessagesWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateDirectMessagesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateDirectMessagesDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateDirectMessagesDialog() override;

    [[nodiscard]] QStringList userNames() const;

private:
    void readConfig();
    void writeConfig();
    void slotAccepted();

    CreateDirectMessagesWidget *const mCreateDirectMessagesWidget;
};
