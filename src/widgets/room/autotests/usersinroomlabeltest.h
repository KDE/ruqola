/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersInRoomLabelTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersInRoomLabelTest(QObject *parent = nullptr);
    ~UsersInRoomLabelTest() override = default;
};
