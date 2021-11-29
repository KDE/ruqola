/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class AddUsersInRoomWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersInRoomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddUsersInRoomDialog(QWidget *parent = nullptr);
    ~AddUsersInRoomDialog() override;
    Q_REQUIRED_RESULT QStringList userIds() const;
    Q_REQUIRED_RESULT QStringList userNames() const;

private:
    void readConfig();
    void writeConfig();
    AddUsersInRoomWidget *const mAddUsersInRoomWidget;
};

