/*
   SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AccountsChannelsModelTest : public QObject
{
    Q_OBJECT
public:
    explicit AccountsChannelsModelTest(QObject *parent = nullptr);

private Q_SLOTS:
    void initTestCase();
    void accountsAndChannels();
};

