/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class OtrManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit OtrManagerTest(QObject *parent = nullptr);
    ~OtrManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

