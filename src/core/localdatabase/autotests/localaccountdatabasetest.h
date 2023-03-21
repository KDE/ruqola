/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalAccountDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit LocalAccountDatabaseTest(QObject *parent = nullptr);
    ~LocalAccountDatabaseTest() override = default;
private Q_SLOTS:
    void initTestCase();
    void shouldHaveDefaultValues();
};
