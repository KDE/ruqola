/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersListJobTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersListJobTest(QObject *parent = nullptr);
    ~UsersListJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateRequestUsername();
};

