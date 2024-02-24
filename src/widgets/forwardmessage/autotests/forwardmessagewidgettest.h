/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ForwardMessageWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ForwardMessageWidgetTest(QObject *parent = nullptr);
    ~ForwardMessageWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
