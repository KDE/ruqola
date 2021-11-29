/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AddUsersWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AddUsersWidgetTest(QObject *parent = nullptr);
    ~AddUsersWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

