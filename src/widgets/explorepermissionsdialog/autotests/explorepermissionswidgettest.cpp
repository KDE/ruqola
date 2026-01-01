/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "explorepermissionsdialog/explorepermissionswidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QTest>
#include <QTreeView>
#include <QVBoxLayout>

QTEST_MAIN(ExplorePermissionsWidgetTest)
ExplorePermissionsWidgetTest::ExplorePermissionsWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExplorePermissionsWidgetTest::shouldHaveDefaultValues()
{
    ExplorePermissionsWidget w;
    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());

    auto mPermissionFilterProxyModel = w.findChild<QSortFilterProxyModel *>(u"permissionFilterProxyModel"_s);
    QVERIFY(mPermissionFilterProxyModel);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mOwnRoles = w.findChild<QLabel *>(u"mOwnRoles"_s);
    QVERIFY(mOwnRoles);
    QVERIFY(mOwnRoles->text().isEmpty());
    QCOMPARE(mOwnRoles->textInteractionFlags(), Qt::TextBrowserInteraction);
}

#include "moc_explorepermissionswidgettest.cpp"
