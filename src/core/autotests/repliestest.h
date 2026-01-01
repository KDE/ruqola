/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RepliesTest : public QObject
{
    Q_OBJECT
public:
    explicit RepliesTest(QObject *parent = nullptr);
    ~RepliesTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
