/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoleEditDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit RoleEditDialogTest(QObject *parent = nullptr);
    ~RoleEditDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
