/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class E2EDataBaseTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testSaveLoadDelete();
    void testOverwrite();
    void testNonExistentKey();
    void shouldDefaultValues();
};
