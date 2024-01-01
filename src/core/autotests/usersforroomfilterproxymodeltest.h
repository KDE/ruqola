/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersForRoomFilterProxyModelTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersForRoomFilterProxyModelTest(QObject *parent = nullptr);
    ~UsersForRoomFilterProxyModelTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldAssignValue();
};
