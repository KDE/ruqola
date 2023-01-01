/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RolesTest : public QObject
{
    Q_OBJECT
public:
    explicit RolesTest(QObject *parent = nullptr);
    ~RolesTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldFindRoles();
    void shouldLoadRoles();
    void shouldLoadRoles_data();
};
