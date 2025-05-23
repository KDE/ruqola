/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NotifyAdminsAppsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit NotifyAdminsAppsJobTest(QObject *parent = nullptr);
    ~NotifyAdminsAppsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
