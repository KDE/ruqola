/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NotificationInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit NotificationInfoTest(QObject *parent = nullptr);
    ~NotificationInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldParseNotification();
    void shouldParseNotification_data();
};
