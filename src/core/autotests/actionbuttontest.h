/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ActionButtonTest : public QObject
{
    Q_OBJECT
public:
    explicit ActionButtonTest(QObject *parent = nullptr);
    ~ActionButtonTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadActionButton();
    void shouldLoadActionButton_data();
};
