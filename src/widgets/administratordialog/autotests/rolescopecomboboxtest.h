/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoleScopeComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit RoleScopeComboBoxTest(QObject *parent = nullptr);
    ~RoleScopeComboBoxTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
