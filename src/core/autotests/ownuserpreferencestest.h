/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class OwnUserPreferencesTest : public QObject
{
    Q_OBJECT
public:
    explicit OwnUserPreferencesTest(QObject *parent = nullptr);
    ~OwnUserPreferencesTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadOwnUserPreferencesInfo_data();
    void shouldLoadOwnUserPreferencesInfo();
};
