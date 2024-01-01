/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class BlockTest : public QObject
{
    Q_OBJECT
public:
    explicit BlockTest(QObject *parent = nullptr);
    ~BlockTest() override = default;

private Q_SLOTS:
    void shouldSerializeData();
    void shouldLoadBlock();
    void shouldLoadBlock_data();
    void shouldHaveDefaultValues();
};
