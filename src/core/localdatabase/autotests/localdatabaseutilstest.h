/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalDatabaseUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit LocalDatabaseUtilsTest(QObject *parent = nullptr);
    ~LocalDatabaseUtilsTest() override = default;
private Q_SLOTS:
    void shouldAdaptRoomName();
    void shouldCheckPath();
    void shouldCheckDataBase();
};
