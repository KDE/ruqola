/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "showlistmessagebasedialog.h"

class ShowPinnedMessagesDialog : public ShowListMessageBaseDialog
{
    Q_OBJECT
public:
    explicit ShowPinnedMessagesDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowPinnedMessagesDialog() override;

private:
    void readConfig();
    void writeConfig();
};
