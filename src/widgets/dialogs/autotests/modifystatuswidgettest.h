/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModifyStatusWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ModifyStatusWidgetTest(QObject *parent = nullptr);
    ~ModifyStatusWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void changeStatusMessage();
};
