/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ManageLocalDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit ManageLocalDatabaseTest(QObject *parent = nullptr);
    ~ManageLocalDatabaseTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
