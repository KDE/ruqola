/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class StatusModelTest : public QObject
{
    Q_OBJECT
public:
    explicit StatusModelTest(QObject *parent = nullptr);
    ~StatusModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
