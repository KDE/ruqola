/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AdministratorCustomSoundsCreateDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorCustomSoundsCreateDialogTest(QObject *parent = nullptr);
    ~AdministratorCustomSoundsCreateDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
