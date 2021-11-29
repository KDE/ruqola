/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CommandsModelTest : public QObject
{
    Q_OBJECT
public:
    explicit CommandsModelTest(QObject *parent = nullptr);
    ~CommandsModelTest() override = default;
};

