/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersInRoomTreeViewTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersInRoomTreeViewTest(QObject *parent = nullptr);
    ~UsersInRoomTreeViewTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
