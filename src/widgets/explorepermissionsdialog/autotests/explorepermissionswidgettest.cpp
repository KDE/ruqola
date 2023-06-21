/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidgettest.h"
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
    auto mTreeView = w.findChild<QTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());

    auto mPermissionFilterProxyModel = w.findChild<QSortFilterProxyModel *>(QStringLiteral("permissionFilterProxyModel"));
    QVERIFY(mPermissionFilterProxyModel);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mOwnRoles = w.findChild<QLabel *>(QStringLiteral("mOwnRoles"));
    QVERIFY(mOwnRoles);
    QVERIFY(mOwnRoles->text().isEmpty());
    QCOMPARE(mOwnRoles->textInteractionFlags(), Qt::TextBrowserInteraction);
}

#include "moc_explorepermissionswidgettest.cpp"
