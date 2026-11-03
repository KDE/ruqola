/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "categories/category.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QListView;
class QModelIndex;
class QSortFilterProxyModel;
class QStandardItem;
class QStandardItemModel;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ManageCategoriesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ManageCategoriesWidget(QWidget *parent = nullptr);
    ~ManageCategoriesWidget() override;

    void setCategories(const QList<Category> &categories);
    [[nodiscard]] QList<Category> categories() const;

private:
    enum CategoryRoles {
        CategoryRole = Qt::UserRole + 1,
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddCategory();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyCategory();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyCategory(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveCategory();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateButtons();
    LIBRUQOLAWIDGETS_NO_EXPORT void addCategory(const Category &category);
    LIBRUQOLAWIDGETS_NO_EXPORT static void updateItem(QStandardItem *item, const Category &category);
    QListView *const mCategoriesListView;
    QStandardItemModel *const mCategoriesModel;
    QSortFilterProxyModel *const mCategoriesProxyModel;
    QToolButton *const mAddButton;
    QToolButton *const mModifyButton;
    QToolButton *const mRemoveButton;
};
