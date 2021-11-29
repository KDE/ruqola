/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CreateNewServerWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateNewServerWidgetTest(QObject *parent = nullptr);
    ~CreateNewServerWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

