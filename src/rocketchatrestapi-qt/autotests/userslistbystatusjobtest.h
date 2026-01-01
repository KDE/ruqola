/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersListByStatusJobTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersListByStatusJobTest(QObject *parent = nullptr);
    ~UsersListByStatusJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldUsersListByStatusJobInfoValue();
};
