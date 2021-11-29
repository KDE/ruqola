/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChangeGroupsReadonlyJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChangeGroupsReadonlyJobTest(QObject *parent = nullptr);
    ~ChangeGroupsReadonlyJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};

