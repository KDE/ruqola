/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AwayManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit AwayManagerTest(QObject *parent = nullptr);
    ~AwayManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
