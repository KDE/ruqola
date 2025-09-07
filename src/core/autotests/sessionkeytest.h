/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SessionKeyTest : public QObject
{
    Q_OBJECT
public:
    explicit SessionKeyTest(QObject *parent = nullptr);
    ~SessionKeyTest() override = default;

private Q_SLOTS:
    void sessionKeyGenerationTest();
    void sessionKeyEncryptionDecryptionTest();
};
