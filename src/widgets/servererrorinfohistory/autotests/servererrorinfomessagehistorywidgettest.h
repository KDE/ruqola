/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ServerErrorInfoMessageHistoryWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryWidgetTest(QObject *parent = nullptr);
    ~ServerErrorInfoMessageHistoryWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
