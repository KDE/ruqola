/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class InputCompleterModelTest : public QObject
{
    Q_OBJECT
public:
    explicit InputCompleterModelTest(QObject *parent = nullptr);
    ~InputCompleterModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValues();
    void shouldLoadValueFromJson();
    void shouldClearModel();
};
