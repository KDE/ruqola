/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class CreateUpdateUserInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateUpdateUserInfoTest(QObject *parent = nullptr);
    ~CreateUpdateUserInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
