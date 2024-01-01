/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SyncMessagesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit SyncMessagesJobTest(QObject *parent = nullptr);
    ~SyncMessagesJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
