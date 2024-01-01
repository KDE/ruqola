/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoruserswidgettest.h"
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
    AdministratorUsersWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mResultTreeView = w.findChild<QTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mResultTreeView);
    QVERIFY(!mResultTreeView->rootIsDecorated());
    QVERIFY(mResultTreeView->isSortingEnabled());

    auto mSearchLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());
}

#include "moc_administratoruserswidgettest.cpp"
