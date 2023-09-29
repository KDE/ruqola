/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ListCommandsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ListCommandsJobTest(QObject *parent = nullptr);
    ~ListCommandsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
