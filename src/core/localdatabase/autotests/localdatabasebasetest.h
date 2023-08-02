/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalDatabaseBaseTest : public QObject
{
    Q_OBJECT
public:
    explicit LocalDatabaseBaseTest(QObject *parent = nullptr);
    ~LocalDatabaseBaseTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
