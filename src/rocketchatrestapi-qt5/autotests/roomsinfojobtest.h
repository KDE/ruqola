/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomsInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomsInfoJobTest(QObject *parent = nullptr);
    ~RoomsInfoJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
