/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationDismissUserReportsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationDismissUserReportsJobTest(QObject *parent = nullptr);
    ~ModerationDismissUserReportsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
