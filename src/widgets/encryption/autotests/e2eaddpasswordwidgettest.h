/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class E2eAddPasswordWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eAddPasswordWidgetTest(QObject *parent = nullptr);
    ~E2eAddPasswordWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSaveSignal();
};
