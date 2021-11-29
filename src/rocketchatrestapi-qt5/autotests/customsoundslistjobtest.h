/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CustomSoundsListJobTest : public QObject
{
    Q_OBJECT
public:
    explicit CustomSoundsListJobTest(QObject *parent = nullptr);
    ~CustomSoundsListJobTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};

