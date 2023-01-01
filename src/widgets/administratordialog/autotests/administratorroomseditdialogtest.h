/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class AdministratorRoomsEditDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorRoomsEditDialogTest(QObject *parent = nullptr);
    ~AdministratorRoomsEditDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
