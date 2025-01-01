/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GroupsDeleteJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GroupsDeleteJobTest(QObject *parent = nullptr);
    ~GroupsDeleteJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateRoomIdJson();
};
