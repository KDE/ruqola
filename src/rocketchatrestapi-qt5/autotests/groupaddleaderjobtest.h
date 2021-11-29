/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GroupAddLeaderJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GroupAddLeaderJobTest(QObject *parent = nullptr);
    ~GroupAddLeaderJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};

