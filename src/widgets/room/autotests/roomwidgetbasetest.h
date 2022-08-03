/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomWidgetBaseTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomWidgetBaseTest(QObject *parent = nullptr);
    ~RoomWidgetBaseTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
