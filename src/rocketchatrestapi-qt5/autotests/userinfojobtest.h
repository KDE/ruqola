/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UserInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit UserInfoJobTest(QObject *parent = nullptr);
    ~UserInfoJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateRequestUsername();
};
