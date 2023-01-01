/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SystemMessagesModelTest : public QObject
{
    Q_OBJECT
public:
    explicit SystemMessagesModelTest(QObject *parent = nullptr);
    ~SystemMessagesModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
