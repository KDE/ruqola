/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class VerifyNewVersionWidgetActionTest : public QObject
{
    Q_OBJECT
public:
    explicit VerifyNewVersionWidgetActionTest(QObject *parent = nullptr);
    ~VerifyNewVersionWidgetActionTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
