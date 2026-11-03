/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managecategorieswidget.h"
#include "categorydialog.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHBoxLayout>
#include <QListView>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QToolButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

namespace
{
class HideDefaultCategoriesProxyModel : public QSortFilterProxyModel
{
public:
    explicit HideDefaultCategoriesProxyModel(int categoryRole, QObject *parent)
        : QSortFilterProxyModel(parent)
        , mCategoryRole(categoryRole)
    {
    }

protected:
    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        // Hide default categories we don't want to see them.
        const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        return !index.data(mCategoryRole).value<Category>().defaultCategory();
    }

private:
    const int mCategoryRole;
};
}

ManageCategoriesWidget::ManageCategoriesWidget(QWidget *parent)
    : QWidget{parent}
    , mCategoriesListView(new QListView(this))
    , mCategoriesModel(new QStandardItemModel(this))
    , mCategoriesProxyModel(new HideDefaultCategoriesProxyModel(CategoryRole, this))
    , mAddButton(new QToolButton(this))
    , mModifyButton(new QToolButton(this))
    , mRemoveButton(new QToolButton(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mCategoriesListView->setObjectName(u"mCategoriesListView"_s);
    mCategoriesProxyModel->setSourceModel(mCategoriesModel);
    mCategoriesListView->setModel(mCategoriesProxyModel);
    mCategoriesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(mCategoriesListView);
    connect(mCategoriesListView, &QListView::doubleClicked, this, &ManageCategoriesWidget::slotModifyCategoryFromIndex);
    connect(mCategoriesListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ManageCategoriesWidget::updateButtons);

    auto buttonLayout = new QVBoxLayout;
    buttonLayout->setObjectName(u"buttonLayout"_s);
    buttonLayout->setContentsMargins({});
    mainLayout->addLayout(buttonLayout);

    mAddButton->setObjectName(u"mAddButton"_s);
    mAddButton->setIcon(QIcon::fromTheme(u"list-add"_s));
    mAddButton->setToolTip(i18nc("@info:tooltip", "Add Category…"));
    mAddButton->setAutoRaise(true);
    buttonLayout->addWidget(mAddButton);
    connect(mAddButton, &QToolButton::clicked, this, &ManageCategoriesWidget::slotAddCategory);

    mModifyButton->setObjectName(u"mModifyButton"_s);
    mModifyButton->setIcon(QIcon::fromTheme(u"edit-rename"_s));
    mModifyButton->setToolTip(i18nc("@info:tooltip", "Rename Category…"));
    mModifyButton->setAutoRaise(true);
    buttonLayout->addWidget(mModifyButton);
    connect(mModifyButton, &QToolButton::clicked, this, &ManageCategoriesWidget::slotModifyCategory);

    mRemoveButton->setObjectName(u"mRemoveButton"_s);
    mRemoveButton->setIcon(QIcon::fromTheme(u"list-remove"_s));
    mRemoveButton->setToolTip(i18nc("@info:tooltip", "Remove Category"));
    mRemoveButton->setAutoRaise(true);
    buttonLayout->addWidget(mRemoveButton);
    connect(mRemoveButton, &QToolButton::clicked, this, &ManageCategoriesWidget::slotRemoveCategory);

    buttonLayout->addStretch(1);

    updateButtons();
}

ManageCategoriesWidget::~ManageCategoriesWidget() = default;

void ManageCategoriesWidget::setCategories(const QList<Category> &categories)
{
    mCategoriesModel->clear();
    for (const Category &category : categories) {
        addCategory(category);
    }
    updateButtons();
}

QList<Category> ManageCategoriesWidget::categories() const
{
    QList<Category> categories;
    const int rowCount = mCategoriesModel->rowCount();
    categories.reserve(rowCount);
    for (int i = 0; i < rowCount; ++i) {
        categories.append(mCategoriesModel->item(i)->data(CategoryRole).value<Category>());
    }
    return categories;
}

void ManageCategoriesWidget::addCategory(const Category &category)
{
    auto item = new QStandardItem;
    updateItem(item, category);
    mCategoriesModel->appendRow(item);
}

void ManageCategoriesWidget::updateItem(QStandardItem *item, const Category &category)
{
    item->setText(category.name());
    item->setData(QVariant::fromValue(category), CategoryRole);
}

void ManageCategoriesWidget::slotAddCategory()
{
    QPointer<CategoryDialog> dialog(new CategoryDialog(CategoryDialog::Mode::Add, this));
    if (dialog->exec()) {
        Category category;
        category.setName(dialog->categoryName());
        addCategory(category);
    }
    delete dialog;
}

void ManageCategoriesWidget::slotModifyCategory()
{
    slotModifyCategoryFromIndex(mCategoriesListView->currentIndex());
}

void ManageCategoriesWidget::slotModifyCategoryFromIndex(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QStandardItem *item = mCategoriesModel->itemFromIndex(mCategoriesProxyModel->mapToSource(index));
    // copy => we keep identifier
    Category category = item->data(CategoryRole).value<Category>();
    QPointer<CategoryDialog> dialog(new CategoryDialog(CategoryDialog::Mode::Rename, this));
    dialog->setCategoryName(category.name());
    if (dialog->exec()) {
        category.setName(dialog->categoryName());
        updateItem(item, category);
    }
    delete dialog;
}

void ManageCategoriesWidget::slotRemoveCategory()
{
    const QModelIndex index = mCategoriesListView->currentIndex();
    if (!index.isValid()) {
        return;
    }
    if (KMessageBox::questionTwoActions(this,
                                        i18n("Do you want to remove this category (%1)?", index.data().toString()),
                                        i18nc("@title:window", "Remove Category"),
                                        KStandardGuiItem::remove(),
                                        KStandardGuiItem::cancel())
        == KMessageBox::PrimaryAction) {
        mCategoriesModel->removeRow(mCategoriesProxyModel->mapToSource(index).row());
        updateButtons();
    }
}

void ManageCategoriesWidget::updateButtons()
{
    const bool hasSelection = mCategoriesListView->selectionModel()->hasSelection();
    mModifyButton->setEnabled(hasSelection);
    mRemoveButton->setEnabled(hasSelection);
}

#include "moc_managecategorieswidget.cpp"
