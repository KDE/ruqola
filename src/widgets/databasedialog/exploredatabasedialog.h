/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ExploreDatabaseWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExploreDatabaseDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseDialog() override;

private:
    void readConfig();
    void writeConfig();
    ExploreDatabaseWidget *const mExploreDatabaseWidget;
};
