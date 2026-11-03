/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managecategorieswidgettest.h"
#include "category/managecategorieswidget.h"
#include <QHBoxLayout>
#include <QListView>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ManageCategoriesWidgetTest)
namespace
{
Category createCategory(const QByteArray &identifier, const QString &name)
{
    Category category;
    category.setIdentifier(identifier);
    category.setName(name);
    return category;
}
}
ManageCategoriesWidgetTest::ManageCategoriesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ManageCategoriesWidgetTest::shouldHaveDefaultValues()
{
    const ManageCategoriesWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto buttonLayout = w.findChild<QVBoxLayout *>(u"buttonLayout"_s);
    QVERIFY(buttonLayout);
    QCOMPARE(buttonLayout->contentsMargins(), QMargins{});

    auto mCategoriesListView = w.findChild<QListView *>(u"mCategoriesListView"_s);
    QVERIFY(mCategoriesListView);
    QVERIFY(mCategoriesListView->model());
    QCOMPARE(mCategoriesListView->model()->rowCount(), 0);

    auto mAddButton = w.findChild<QToolButton *>(u"mAddButton"_s);
    QVERIFY(mAddButton);
    QVERIFY(mAddButton->isEnabled());
    QVERIFY(!mAddButton->toolTip().isEmpty());

    auto mModifyButton = w.findChild<QToolButton *>(u"mModifyButton"_s);
    QVERIFY(mModifyButton);
    QVERIFY(!mModifyButton->isEnabled());
    QVERIFY(!mModifyButton->toolTip().isEmpty());

    auto mRemoveButton = w.findChild<QToolButton *>(u"mRemoveButton"_s);
    QVERIFY(mRemoveButton);
    QVERIFY(!mRemoveButton->isEnabled());
    QVERIFY(!mRemoveButton->toolTip().isEmpty());

    QVERIFY(w.categories().isEmpty());
}

void ManageCategoriesWidgetTest::shouldStoreCategories()
{
    ManageCategoriesWidget w;
    Category category2 = createCategory("category2", u"name-category2"_s);
    category2.setShowUnreads(true);
    category2.setDefaultCategory(true);
    const QList<Category> categories{createCategory("category1", u"name-category1"_s), category2};
    w.setCategories(categories);

    auto mCategoriesListView = w.findChild<QListView *>(u"mCategoriesListView"_s);
    QVERIFY(mCategoriesListView);
    // The default category is hidden but kept
    QCOMPARE(mCategoriesListView->model()->rowCount(), 1);
    QCOMPARE(mCategoriesListView->model()->index(0, 0).data().toString(), u"name-category1"_s);
    QCOMPARE(w.categories(), categories);
}

void ManageCategoriesWidgetTest::shouldEnableButtonsWhenSelectingCategory()
{
    ManageCategoriesWidget w;
    w.setCategories({createCategory("category1", u"name-category1"_s)});

    auto mCategoriesListView = w.findChild<QListView *>(u"mCategoriesListView"_s);
    QVERIFY(mCategoriesListView);
    auto mModifyButton = w.findChild<QToolButton *>(u"mModifyButton"_s);
    QVERIFY(mModifyButton);
    auto mRemoveButton = w.findChild<QToolButton *>(u"mRemoveButton"_s);
    QVERIFY(mRemoveButton);
    QVERIFY(!mModifyButton->isEnabled());
    QVERIFY(!mRemoveButton->isEnabled());

    // WHEN
    mCategoriesListView->setCurrentIndex(mCategoriesListView->model()->index(0, 0));

    // THEN
    QVERIFY(mModifyButton->isEnabled());
    QVERIFY(mRemoveButton->isEnabled());

    // WHEN the model is reset, the selection is gone again.
    w.setCategories({});

    // THEN
    QVERIFY(!mModifyButton->isEnabled());
    QVERIFY(!mRemoveButton->isEnabled());
}

#include "moc_managecategorieswidgettest.cpp"
