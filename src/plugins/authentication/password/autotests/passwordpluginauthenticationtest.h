/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PasswordPluginAuthenticationTest : public QObject
{
    Q_OBJECT
public:
    explicit PasswordPluginAuthenticationTest(QObject *parent = nullptr);
    ~PasswordPluginAuthenticationTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
