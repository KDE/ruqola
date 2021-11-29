/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class AddUsersWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersInRoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddUsersInRoomWidget(QWidget *parent = nullptr);
    ~AddUsersInRoomWidget() override;

    Q_REQUIRED_RESULT QStringList userIds() const;

    Q_REQUIRED_RESULT QStringList userNames() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    AddUsersWidget *const mAddUsersWidget;
};

