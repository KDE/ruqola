/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PermissionsTest : public QObject
{
    Q_OBJECT
public:
    explicit PermissionsTest(QObject *parent = nullptr);
    ~PermissionsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    // void shouldLoadPermissions();
    // void shouldLoadPermissions_data();
};
