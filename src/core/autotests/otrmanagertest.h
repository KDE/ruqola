/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
