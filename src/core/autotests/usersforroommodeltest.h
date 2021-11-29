/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersForRoomModelTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersForRoomModelTest(QObject *parent = nullptr);
    ~UsersForRoomModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddValues();
    void shouldVerifyData();

    void shouldParseUsers_data();
    void shouldParseUsers();
};

