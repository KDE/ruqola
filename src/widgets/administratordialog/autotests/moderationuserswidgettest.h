/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationUsersWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationUsersWidgetTest(QObject *parent = nullptr);
    ~ModerationUsersWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
