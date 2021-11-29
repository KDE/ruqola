/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AskTwoAuthenticationPasswordWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AskTwoAuthenticationPasswordWidgetTest(QObject *parent = nullptr);
    ~AskTwoAuthenticationPasswordWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

