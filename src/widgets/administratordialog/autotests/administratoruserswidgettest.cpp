/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoruserswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/users/administratoruserswidget.h"
#include <QLineEdit>
#include <QTest>
#include <QTreeView>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorUsersWidgetTest)

AdministratorUsersWidgetTest::AdministratorUsersWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorUsersWidgetTest::shouldHaveDefaultValues()
{
    AdministratorUsersWidget w(AdministratorUsersWidget::UsersType::All, nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    auto mResultTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);
    QVERIFY(mResultTreeView);
    QVERIFY(!mResultTreeView->rootIsDecorated());
    QVERIFY(mResultTreeView->isSortingEnabled());

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());
}

#include "moc_administratoruserswidgettest.cpp"
