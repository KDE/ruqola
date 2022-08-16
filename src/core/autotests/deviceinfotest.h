/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DeviceInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfoTest(QObject *parent = nullptr);
    ~DeviceInfoTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldDeviceInfo();
    void shouldDeviceInfo_data();
};
