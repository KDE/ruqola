/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationDismissReportsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationDismissReportsJobTest(QObject *parent = nullptr);
    ~ModerationDismissReportsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
