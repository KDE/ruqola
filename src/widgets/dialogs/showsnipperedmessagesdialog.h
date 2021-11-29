/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "showlistmessagebasedialog.h"

class ShowSnipperedMessagesDialog : public ShowListMessageBaseDialog
{
    Q_OBJECT
public:
    explicit ShowSnipperedMessagesDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowSnipperedMessagesDialog() override;

private:
    void readConfig();
    void writeConfig();
};

