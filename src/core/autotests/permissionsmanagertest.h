/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PermissionsManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit PermissionsManagerTest(QObject *parent = nullptr);
    ~PermissionsManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldHasRoles();

    void shouldLoadPermissions();
    void shouldLoadPermissions_data();

    void shouldUpdatePermissions();
    void shouldUpdatePermissions_data();
};
