/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QComboBox>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit UsersInRoomComboBox(QWidget *parent = nullptr);
    ~UsersInRoomComboBox() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void fillCombobox();
};
