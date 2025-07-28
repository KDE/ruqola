/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ExploreDatabaseJsonWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ExploreDatabaseJsonWidgetTest(QObject *parent = nullptr);
    ~ExploreDatabaseJsonWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
