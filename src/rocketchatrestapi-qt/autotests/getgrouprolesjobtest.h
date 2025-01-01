/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GetGroupRolesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GetGroupRolesJobTest(QObject *parent = nullptr);
    ~GetGroupRolesJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
