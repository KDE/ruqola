/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomHeaderWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomHeaderWidgetTest(QObject *parent = nullptr);
    ~RoomHeaderWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldShowHideIcon();
    void shouldEmitSignal();
};

