/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationReportInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationReportInfoTest(QObject *parent = nullptr);
    ~ModerationReportInfoTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadReportInfo_data();
    void shouldLoadReportInfo();
};
