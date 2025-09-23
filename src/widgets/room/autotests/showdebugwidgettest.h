/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <QObject>

class ShowDebugWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ShowDebugWidgetTest(QObject *parent = nullptr);
    ~ShowDebugWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
