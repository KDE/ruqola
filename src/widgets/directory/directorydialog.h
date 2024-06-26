/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>

#include "directorywidget.h"
#include "libruqolawidgets_private_export.h"
class DirectoryTabWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DirectoryDialog(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryDialog() override;

    void fillTabs();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    DirectoryTabWidget *const mDirectoryTabWidget;
};
