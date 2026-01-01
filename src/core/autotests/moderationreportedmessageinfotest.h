/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationReportedMessageInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationReportedMessageInfoTest(QObject *parent = nullptr);
    ~ModerationReportedMessageInfoTest() override = default;

private Q_SLOTS:
    void shouldModerationInfo_data();
    void shouldModerationInfo();
    void shouldHaveDefaultValues();
};
