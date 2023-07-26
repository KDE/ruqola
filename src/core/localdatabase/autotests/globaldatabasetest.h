/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GlobalDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit GlobalDatabaseTest(QObject *parent = nullptr);
    ~GlobalDatabaseTest() override = default;

private Q_SLOTS:
    void initTestCase();
    void shouldStoreIdentifier();

    void shouldGenerateIdentifier();
    void shouldGenerateIdentifier_data();
};
