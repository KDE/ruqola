/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UserInRoleEditDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit UserInRoleEditDialogTest(QObject *parent = nullptr);
    ~UserInRoleEditDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
