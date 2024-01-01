/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationUserDeleteReportedMessagesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationUserDeleteReportedMessagesJobTest(QObject *parent = nullptr);
    ~ModerationUserDeleteReportedMessagesJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
