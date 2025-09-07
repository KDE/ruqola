/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageEncryptionDecryptionTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageEncryptionDecryptionTest(QObject *parent = nullptr);
    ~MessageEncryptionDecryptionTest() override = default;

private Q_SLOTS:
    void messageEncryptionDecryptionTest();
};
