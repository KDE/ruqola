/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationReportsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationReportsJobTest(QObject *parent = nullptr);
    ~ModerationReportsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
