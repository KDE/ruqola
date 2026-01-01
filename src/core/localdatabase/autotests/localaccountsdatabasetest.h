/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalAccountsDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit LocalAccountsDatabaseTest(QObject *parent = nullptr);
    ~LocalAccountsDatabaseTest() override = default;
private Q_SLOTS:
    void initTestCase();
    void shouldHaveDefaultValues();
    void shouldStoreAccountSettings();
    void shouldRemoveAccountSettings();
    void shouldVerifyDbFileName();
};
