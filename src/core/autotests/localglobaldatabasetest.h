/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalGlobalDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit LocalGlobalDatabaseTest(QObject *parent = nullptr);
    ~LocalGlobalDatabaseTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
