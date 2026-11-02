/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "categories/category.h"
#include "libruqolawidgets_private_export.h"
#include <QMenu>
class QActionGroup;
/**
 * A menu which moves a room to a category: one exclusive checkable action per category, plus an entry
 * which removes the room from its category and one which opens the categories management dialog.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT CategoriesMenu : public QMenu
{
    Q_OBJECT
public:
    explicit CategoriesMenu(QWidget *parent = nullptr);
    ~CategoriesMenu() override;

    void setCategories(const QList<Category> &categories);
    [[nodiscard]] QList<Category> categories() const;

    /*! Identifier of the category of the current room, empty when it doesn't belong to a category. */
    void setCurrentCategory(const QByteArray &identifier);
    [[nodiscard]] QByteArray currentCategory() const;

Q_SIGNALS:
    /*! Emitted when the user moves the room to the category \a identifier (empty: no category). */
    void assignCategory(const QByteArray &identifier);
    /*! Emitted when the user edited the categories in the management dialog. */
    void categoriesChanged(const QList<Category> &categories);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateMenu();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAssignCategory(const QByteArray &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotManageCategories();
    QList<Category> mCategories;
    QByteArray mCurrentCategory;
    QActionGroup *const mCategoriesGroup;
};
