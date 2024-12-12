/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AppInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit AppInfoJobTest(QObject *parent = nullptr);
    ~AppInfoJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
