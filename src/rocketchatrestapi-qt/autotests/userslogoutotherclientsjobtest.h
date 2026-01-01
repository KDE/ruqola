/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersLogoutOtherClientsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersLogoutOtherClientsJobTest(QObject *parent = nullptr);
    ~UsersLogoutOtherClientsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
