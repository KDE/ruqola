/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GroupsInviteJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GroupsInviteJobTest(QObject *parent = nullptr);
    ~GroupsInviteJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateUsernameJson();
    void shouldGenerateUserIdJson();
};
