/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RolesManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit RolesManagerTest(QObject *parent = nullptr);
    ~RolesManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();

    void shouldLoadRoles_data();
    void shouldLoadRoles();

    void shouldUpdateRoles_data();
    void shouldUpdateRoles();
};
