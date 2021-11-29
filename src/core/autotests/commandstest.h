/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CommandsTest : public QObject
{
    Q_OBJECT
public:
    explicit CommandsTest(QObject *parent = nullptr);
    ~CommandsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldLoadCommands();
    void shouldLoadCommands_data();
    void shouldLoadPermissions();
};

