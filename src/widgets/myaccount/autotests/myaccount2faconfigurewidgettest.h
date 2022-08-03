/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MyAccount2FaConfigureWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit MyAccount2FaConfigureWidgetTest(QObject *parent = nullptr);
    ~MyAccount2FaConfigureWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
