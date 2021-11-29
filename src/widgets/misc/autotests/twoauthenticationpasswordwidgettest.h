/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TwoAuthenticationPasswordWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TwoAuthenticationPasswordWidgetTest(QObject *parent = nullptr);
    ~TwoAuthenticationPasswordWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

