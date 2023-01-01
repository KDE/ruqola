/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LoginMethodModelTest : public QObject
{
    Q_OBJECT
public:
    explicit LoginMethodModelTest(QObject *parent = nullptr);
    ~LoginMethodModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldClearModel();
    void shouldAssignValues();

    void shouldEmitLoginMethodChanged();
};
