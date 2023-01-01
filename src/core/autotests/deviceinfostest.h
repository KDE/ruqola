/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DeviceInfosTest : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfosTest(QObject *parent = nullptr);
    ~DeviceInfosTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadDiscussions_data();
    void shouldLoadDiscussions();
};
