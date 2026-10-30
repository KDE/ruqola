/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "categorywidgettest.h"
#include "category/categorywidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(CategoryWidgetTest)
CategoryWidgetTest::CategoryWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CategoryWidgetTest::shouldHaveDefaultValues()
{
    const CategoryWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mCategoryLineEdit = w.findChild<QLineEdit *>(u"mCategoryLineEdit"_s);
    QVERIFY(mCategoryLineEdit);
    QVERIFY(mCategoryLineEdit->text().isEmpty());
    QVERIFY(mCategoryLineEdit->isClearButtonEnabled());
    QCOMPARE(label->buddy(), mCategoryLineEdit);

    QVERIFY(w.categoryName().isEmpty());
}

void CategoryWidgetTest::shouldEmitSignal()
{
    const CategoryWidget w;
    QSignalSpy spyUpdateButton(&w, &CategoryWidget::updateOkButton);
    auto mCategoryLineEdit = w.findChild<QLineEdit *>(u"mCategoryLineEdit"_s);

    mCategoryLineEdit->setText(u"foo"_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(spyUpdateButton.at(0).at(0).toBool());

    // an empty text
    spyUpdateButton.clear();
    mCategoryLineEdit->setText({});
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(!spyUpdateButton.at(0).at(0).toBool());

    // a trimmed text empty
    spyUpdateButton.clear();
    mCategoryLineEdit->setText(u"   "_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(!spyUpdateButton.at(0).at(0).toBool());

    spyUpdateButton.clear();
    mCategoryLineEdit->setText(u" bla "_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(spyUpdateButton.at(0).at(0).toBool());
}

void CategoryWidgetTest::shouldSetCategoryName()
{
    CategoryWidget w;
    QSignalSpy spyUpdateButton(&w, &CategoryWidget::updateOkButton);
    w.setCategoryName(u" foo "_s);
    QCOMPARE(w.categoryName(), u"foo"_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(spyUpdateButton.at(0).at(0).toBool());
}

#include "moc_categorywidgettest.cpp"
