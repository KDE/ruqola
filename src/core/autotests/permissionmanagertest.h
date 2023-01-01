/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PermissionManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit PermissionManagerTest(QObject *parent = nullptr);
    ~PermissionManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldHasRoles();

    void shouldLoadPermissions();
    void shouldLoadPermissions_data();

    void shouldUpdatePermissions();
    void shouldUpdatePermissions_data();
};
