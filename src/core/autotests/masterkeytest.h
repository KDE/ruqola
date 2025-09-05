/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MasterKeyTest : public QObject
{
    Q_OBJECT
public:
    explicit MasterKeyTest(QObject *parent = nullptr);
    ~MasterKeyTest() override = default;

private Q_SLOTS:
    void masterKeyDeterminismTest();
    void masterKeyEmptyUserIdTest();
    void masterKeyEmptyPasswordTest();
    void importRawKeyTest();
};
