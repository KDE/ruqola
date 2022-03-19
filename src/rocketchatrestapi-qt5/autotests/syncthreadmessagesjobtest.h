/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SyncThreadMessagesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit SyncThreadMessagesJobTest(QObject *parent = nullptr);
    ~SyncThreadMessagesJobTest() override = default;

private Q_SLOTS:
    void shouldGenerateRequest();
    void shouldNotStarting();
    void shouldHaveDefaultValue();
};
