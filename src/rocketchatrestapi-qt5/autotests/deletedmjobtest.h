/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DeleteDmJobTest : public QObject
{
    Q_OBJECT
public:
    explicit DeleteDmJobTest(QObject *parent = nullptr);
    ~DeleteDmJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateUserIdJson();
    void shouldGenerateUserNameJson();
};
