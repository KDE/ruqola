/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomReplyThreadWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomReplyThreadWidgetTest(QObject *parent = nullptr);
    ~RoomReplyThreadWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
