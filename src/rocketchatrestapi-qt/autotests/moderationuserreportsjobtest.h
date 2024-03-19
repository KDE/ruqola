/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationUserReportsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationUserReportsJobTest(QObject *parent = nullptr);
    ~ModerationUserReportsJobTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
