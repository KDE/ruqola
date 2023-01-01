/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RocketChatAccountModelTest : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatAccountModelTest(QObject *parent = nullptr);
    ~RocketChatAccountModelTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldRemoveAccount();
    void shouldAddAccountValue();
    void shouldClearAccounts();
};
