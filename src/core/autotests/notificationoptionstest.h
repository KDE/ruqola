/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NotificationOptionsTest : public QObject
{
    Q_OBJECT
public:
    explicit NotificationOptionsTest(QObject *parent = nullptr);
    ~NotificationOptionsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldParseNotification_data();
    void shouldParseNotification();
};
