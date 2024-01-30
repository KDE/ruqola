/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "showlistmessagebasedialog.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowThreadsDialog : public ShowListMessageBaseDialog
{
    Q_OBJECT
public:
    explicit ShowThreadsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowThreadsDialog() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
};
