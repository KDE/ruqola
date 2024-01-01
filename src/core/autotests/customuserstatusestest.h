/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CustomUserStatusesTest : public QObject
{
    Q_OBJECT
public:
    explicit CustomUserStatusesTest(QObject *parent = nullptr);
    ~CustomUserStatusesTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadUserCustomStatuses_data();
    void shouldLoadUserCustomStatuses();

    void shouldUpdateUserCustomStatuses_data();
    void shouldUpdateUserCustomStatuses();

    void shouldDeleteUserCustomStatuses();
    void shouldDeleteUserCustomStatuses_data();
};
