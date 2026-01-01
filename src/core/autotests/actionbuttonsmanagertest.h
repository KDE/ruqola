/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ActionButtonsManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit ActionButtonsManagerTest(QObject *parent = nullptr);
    ~ActionButtonsManagerTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadActionButtons();
    void shouldLoadActionButtons_data();
};
