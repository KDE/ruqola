/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class TeamSelectDeletedRoomWidgetTest : public QWidget
{
    Q_OBJECT
public:
    explicit TeamSelectDeletedRoomWidgetTest(QWidget *parent = nullptr);
    ~TeamSelectDeletedRoomWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
