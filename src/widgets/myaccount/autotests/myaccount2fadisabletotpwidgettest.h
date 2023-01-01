/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MyAccount2FaDisableTotpWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit MyAccount2FaDisableTotpWidgetTest(QObject *parent = nullptr);
    ~MyAccount2FaDisableTotpWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
