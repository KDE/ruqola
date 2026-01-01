/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ReportUserWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ReportUserWidgetTest(QObject *parent = nullptr);
    ~ReportUserWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
