/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GroupsConvertToTeamJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GroupsConvertToTeamJobTest(QObject *parent = nullptr);
    ~GroupsConvertToTeamJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
