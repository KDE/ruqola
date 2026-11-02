/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managecategoriesdialogtest.h"
#include "category/managecategoriesdialog.h"
#include "category/managecategorieswidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ManageCategoriesDialogTest)
ManageCategoriesDialogTest::ManageCategoriesDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ManageCategoriesDialogTest::shouldHaveDefaultValues()
{
    const ManageCategoriesDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mManageCategoriesWidget = d.findChild<ManageCategoriesWidget *>(u"mManageCategoriesWidget"_s);
    QVERIFY(mManageCategoriesWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(d.categories().isEmpty());
}

void ManageCategoriesDialogTest::shouldStoreCategories()
{
    ManageCategoriesDialog d;
    Category category;
    category.setIdentifier("category1");
    category.setName(u"name-category1"_s);
    const QList<Category> categories{category};
    d.setCategories(categories);
    QCOMPARE(d.categories(), categories);
}

#include "moc_managecategoriesdialogtest.cpp"
