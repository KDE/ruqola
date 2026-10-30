/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CategoryWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit CategoryWidgetTest(QObject *parent = nullptr);
    ~CategoryWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSignal();
    void shouldSetCategoryName();
};
