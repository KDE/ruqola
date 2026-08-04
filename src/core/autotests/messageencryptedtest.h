/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class MessageEncryptedTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageEncryptedTest(QObject *parent = nullptr);
    ~MessageEncryptedTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEncryptV2Payload();
    void shouldDecryptV2Payload();
};
