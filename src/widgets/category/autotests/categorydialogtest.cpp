/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "categorydialogtest.h"
#include "category/categorydialog.h"
#include "category/categorywidget.h"
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(CategoryDialogTest)
CategoryDialogTest::CategoryDialogTest(QObject *parent)
    : QObject{parent}
{
}

void CategoryDialogTest::shouldHaveDefaultValues()
{
    const CategoryDialog d(CategoryDialog::Mode::Add);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCategoryWidget = d.findChild<CategoryWidget *>(u"mCategoryWidget"_s);
    QVERIFY(mCategoryWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
    QPushButton *buttonOk = button->button(QDialogButtonBox::Ok);
    QVERIFY(buttonOk);
    QVERIFY(!buttonOk->isEnabled());
    QVERIFY(button->button(QDialogButtonBox::Cancel));

    QVERIFY(d.categoryName().isEmpty());
}

void CategoryDialogTest::shouldHaveDifferentTitle()
{
    const CategoryDialog add(CategoryDialog::Mode::Add);
    const CategoryDialog rename(CategoryDialog::Mode::Rename);
    QVERIFY(!rename.windowTitle().isEmpty());
    QVERIFY(add.windowTitle() != rename.windowTitle());
}

void CategoryDialogTest::shouldEnableOkButton()
{
    CategoryDialog d(CategoryDialog::Mode::Rename);
    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QPushButton *buttonOk = button->button(QDialogButtonBox::Ok);
    QVERIFY(!buttonOk->isEnabled());

    d.setCategoryName(u"foo"_s);
    QVERIFY(buttonOk->isEnabled());
    QCOMPARE(d.categoryName(), u"foo"_s);

    auto lineEdit = d.findChild<QLineEdit *>(u"mCategoryLineEdit"_s);
    QVERIFY(lineEdit);
    lineEdit->setText(u"  "_s);
    QVERIFY(!buttonOk->isEnabled());

    lineEdit->clear();
    QVERIFY(!buttonOk->isEnabled());

    lineEdit->setText(u"bla"_s);
    QVERIFY(buttonOk->isEnabled());
}

#include "moc_categorydialogtest.cpp"
