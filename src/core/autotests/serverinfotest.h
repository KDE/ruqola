/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ServerInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ServerInfoTest(QObject *parent = nullptr);
    ~ServerInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadServerInfo();
    void shouldLoadServerInfo_data();
};
