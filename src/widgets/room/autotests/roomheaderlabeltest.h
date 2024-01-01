/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class RoomHeaderLabelTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomHeaderLabelTest(QObject *parent = nullptr);
    ~RoomHeaderLabelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
