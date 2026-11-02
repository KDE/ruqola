/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "categories/category.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ManageCategoriesWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ManageCategoriesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManageCategoriesDialog(QWidget *parent = nullptr);
    ~ManageCategoriesDialog() override;

    void setCategories(const QList<Category> &categories);
    [[nodiscard]] QList<Category> categories() const;

private:
    ManageCategoriesWidget *const mManageCategoriesWidget;
};
