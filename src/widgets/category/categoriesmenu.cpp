/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "categoriesmenu.h"
#include "managecategoriesdialog.h"
#include <KLocalizedString>
#include <QActionGroup>
#include <QPointer>
using namespace Qt::Literals::StringLiterals;

CategoriesMenu::CategoriesMenu(QWidget *parent)
    : QMenu(parent)
    , mCategoriesGroup(new QActionGroup(this))
{
    setObjectName(u"CategoriesMenu"_s);
    setTitle(i18nc("@title:menu", "Category"));
    setIcon(QIcon::fromTheme(u"folder"_s));
    updateMenu();
}

CategoriesMenu::~CategoriesMenu() = default;

void CategoriesMenu::setCategories(const QList<Category> &categories)
{
    mCategories = categories;
    updateMenu();
}

QList<Category> CategoriesMenu::categories() const
{
    return mCategories;
}

void CategoriesMenu::setCurrentCategory(const QByteArray &identifier)
{
    mCurrentCategory = identifier;
    updateMenu();
}

QByteArray CategoriesMenu::currentCategory() const
{
    return mCurrentCategory;
}

void CategoriesMenu::updateMenu()
{
    clear();
    if (!mCategories.isEmpty()) {
        // A room belongs to at most one category, so the actions are exclusive.
        auto noCategoryAction = new QAction(i18nc("@action", "No Category"), this);
        noCategoryAction->setObjectName(u"noCategoryAction"_s);
        noCategoryAction->setCheckable(true);
        noCategoryAction->setChecked(mCurrentCategory.isEmpty());
        mCategoriesGroup->addAction(noCategoryAction);
        connect(noCategoryAction, &QAction::triggered, this, [this]() {
            slotAssignCategory({});
        });
        addAction(noCategoryAction);

        addSeparator();

        for (const Category &category : std::as_const(mCategories)) {
            auto categoryAction = new QAction(category.name(), this);
            categoryAction->setCheckable(true);
            categoryAction->setChecked(category.identifier() == mCurrentCategory);
            mCategoriesGroup->addAction(categoryAction);
            const QByteArray identifier = category.identifier();
            connect(categoryAction, &QAction::triggered, this, [this, identifier]() {
                slotAssignCategory(identifier);
            });
            addAction(categoryAction);
        }
        addSeparator();
    }
    auto manageCategoriesAction = new QAction(QIcon::fromTheme(u"folder"_s), i18nc("@action", "Manage Categories…"), this);
    manageCategoriesAction->setObjectName(u"manageCategoriesAction"_s);
    connect(manageCategoriesAction, &QAction::triggered, this, &CategoriesMenu::slotManageCategories);
    addAction(manageCategoriesAction);
}

void CategoriesMenu::slotAssignCategory(const QByteArray &identifier)
{
    if (identifier == mCurrentCategory) {
        return;
    }
    mCurrentCategory = identifier;
    Q_EMIT assignCategory(identifier);
}

void CategoriesMenu::slotManageCategories()
{
    // The menu is parented to the menu which shows it: a popup is a poor dialog parent, use the
    // widget which owns the menus instead.
    QWidget *dialogParent = parentWidget();
    while (auto parentMenu = qobject_cast<QMenu *>(dialogParent)) {
        dialogParent = parentMenu->parentWidget();
    }
    QPointer<ManageCategoriesDialog> dialog(new ManageCategoriesDialog(dialogParent));
    dialog->setCategories(mCategories);
    if (dialog->exec()) {
        mCategories = dialog->categories();
        updateMenu();
        Q_EMIT categoriesChanged(mCategories);
    }
    delete dialog;
}

#include "moc_categoriesmenu.cpp"
