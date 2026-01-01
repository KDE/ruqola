/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageExtraTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageExtraTest(QObject *parent = nullptr);
    ~MessageExtraTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
