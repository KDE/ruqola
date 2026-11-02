/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CategoriesMenuTest : public QObject
{
    Q_OBJECT
public:
    explicit CategoriesMenuTest(QObject *parent = nullptr);
    ~CategoriesMenuTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldShowCategories();
    void shouldEmitAssignCategory();
};
