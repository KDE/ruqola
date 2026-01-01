/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class E2eDisableWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eDisableWidgetTest(QObject *parent = nullptr);
    ~E2eDisableWidgetTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
