/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ManageCategoriesWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ManageCategoriesWidgetTest(QObject *parent = nullptr);
    ~ManageCategoriesWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldStoreCategories();
    void shouldEnableButtonsWhenSelectingCategory();
};
