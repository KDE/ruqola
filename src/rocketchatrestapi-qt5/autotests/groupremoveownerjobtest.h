/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GroupRemoveOwnerJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GroupRemoveOwnerJobTest(QObject *parent = nullptr);
    ~GroupRemoveOwnerJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
