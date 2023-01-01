/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class OtrTest : public QObject
{
    Q_OBJECT
public:
    explicit OtrTest(QObject *parent = nullptr);
    ~OtrTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldParseOtr_data();
    void shouldParseOtr();
};
