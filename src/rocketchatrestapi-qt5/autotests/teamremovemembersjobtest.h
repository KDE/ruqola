/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamRemoveMembersJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamRemoveMembersJobTest(QObject *parent = nullptr);
    ~TeamRemoveMembersJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
