/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class E2ePasswordDecodeKeyWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit E2ePasswordDecodeKeyWidgetTest(QObject *parent = nullptr);
    ~E2ePasswordDecodeKeyWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
