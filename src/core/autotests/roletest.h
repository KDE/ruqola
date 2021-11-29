/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoleTest : public QObject
{
    Q_OBJECT
public:
    explicit RoleTest(QObject *parent = nullptr);
    ~RoleTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldBeValid();
    void shouldLoadRoles_data();
    void shouldLoadRoles();
    void shouldVerifyHasRoles();
};

