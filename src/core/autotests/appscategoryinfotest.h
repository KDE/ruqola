/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AppsCategoryInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit AppsCategoryInfoTest(QObject *parent = nullptr);
    ~AppsCategoryInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
