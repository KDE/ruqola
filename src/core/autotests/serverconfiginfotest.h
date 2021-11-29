/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ServerConfigInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ServerConfigInfoTest(QObject *parent = nullptr);
    ~ServerConfigInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

