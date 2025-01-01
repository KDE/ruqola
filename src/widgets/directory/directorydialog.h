/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>

#include "directorywidget.h"
#include "libruqolawidgets_private_export.h"
class DirectoryContainerWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DirectoryDialog(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryDialog() override;

Q_SIGNALS:
    void updateJoinButton(bool status);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    DirectoryContainerWidget *const mDirectoryContainerWidget;
};
