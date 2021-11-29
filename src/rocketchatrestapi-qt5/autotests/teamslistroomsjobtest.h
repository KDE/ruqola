/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamsListRoomsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamsListRoomsJobTest(QObject *parent = nullptr);
    ~TeamsListRoomsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
