/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "categoriesmenutest.h"
#include "category/categoriesmenu.h"
#include <QSignalSpy>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(CategoriesMenuTest)
namespace
{
Category createCategory(const QByteArray &identifier, const QString &name)
{
    Category category;
    category.setIdentifier(identifier);
    category.setName(name);
    return category;
}

QAction *actionFromText(const QMenu &menu, const QString &text)
{
    const auto actions = menu.actions();
    for (QAction *action : actions) {
        if (action->text() == text) {
            return action;
        }
    }
    return nullptr;
}
}
CategoriesMenuTest::CategoriesMenuTest(QObject *parent)
    : QObject{parent}
{
}

void CategoriesMenuTest::shouldHaveDefaultValues()
{
    const CategoriesMenu m;
    QVERIFY(!m.title().isEmpty());
    QVERIFY(m.categories().isEmpty());
    QVERIFY(m.currentCategory().isEmpty());

    // Without categories, it only offers to manage them.
    QCOMPARE(m.actions().count(), 1);
    QVERIFY(m.findChild<QAction *>(u"manageCategoriesAction"_s));
    QVERIFY(!m.findChild<QAction *>(u"noCategoryAction"_s));
}

void CategoriesMenuTest::shouldShowCategories()
{
    CategoriesMenu m;
    const QList<Category> categories{createCategory("category1", u"name-category1"_s), createCategory("category2", u"name-category2"_s)};
    m.setCategories(categories);
    QCOMPARE(m.categories(), categories);

    // no category + separator + 2 categories + separator + manage
    QCOMPARE(m.actions().count(), 6);
    auto noCategoryAction = m.findChild<QAction *>(u"noCategoryAction"_s);
    QVERIFY(noCategoryAction);
    QVERIFY(noCategoryAction->isChecked());

    // WHEN
    m.setCurrentCategory("category2");

    // THEN
    QCOMPARE(m.currentCategory(), "category2"_ba);
    QVERIFY(!m.findChild<QAction *>(u"noCategoryAction"_s)->isChecked());
    QAction *category2Action = actionFromText(m, u"name-category2"_s);
    QVERIFY(category2Action);
    QVERIFY(category2Action->isChecked());
    QVERIFY(!actionFromText(m, u"name-category1"_s)->isChecked());
}

void CategoriesMenuTest::shouldEmitAssignCategory()
{
    CategoriesMenu m;
    m.setCategories({createCategory("category1", u"name-category1"_s)});
    QSignalSpy assignCategorySpy(&m, &CategoriesMenu::assignCategory);

    QAction *category1Action = actionFromText(m, u"name-category1"_s);
    QVERIFY(category1Action);
    category1Action->trigger();
    QCOMPARE(assignCategorySpy.count(), 1);
    QCOMPARE(assignCategorySpy.at(0).at(0).toByteArray(), "category1"_ba);
    QCOMPARE(m.currentCategory(), "category1"_ba);

    // Selecting the current category again doesn't emit.
    category1Action->trigger();
    QCOMPARE(assignCategorySpy.count(), 1);

    m.findChild<QAction *>(u"noCategoryAction"_s)->trigger();
    QCOMPARE(assignCategorySpy.count(), 2);
    QVERIFY(assignCategorySpy.at(1).at(0).toByteArray().isEmpty());
    QVERIFY(m.currentCategory().isEmpty());
}

#include "moc_categoriesmenutest.cpp"
