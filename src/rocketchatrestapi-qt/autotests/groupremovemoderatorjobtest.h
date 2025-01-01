/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GroupRemoveModeratorJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GroupRemoveModeratorJobTest(QObject *parent = nullptr);
    ~GroupRemoveModeratorJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
