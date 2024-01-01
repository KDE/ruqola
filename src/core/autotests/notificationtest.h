/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NotificationTest : public QObject
{
    Q_OBJECT
public:
    explicit NotificationTest(QObject *parent = nullptr);
    ~NotificationTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeStatus();
};
