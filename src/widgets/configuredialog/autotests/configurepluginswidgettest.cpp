/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidgettest.h"
#include "configuredialog/configurepluginswidget.h"
#include <KTreeWidgetSearchLine>
#include <KTreeWidgetSearchLineWidget>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigurePluginsWidgetTest)

ConfigurePluginsWidgetTest::ConfigurePluginsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigurePluginsWidgetTest::shouldHaveDefaultValues()
{
    ConfigurePluginsWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QCOMPARE(mainLayout->spacing(), 0);

    auto mTreePluginWidget = w.findChild<QTreeWidget *>(QStringLiteral("mTreePluginWidget"));
    QVERIFY(mTreePluginWidget);
    QVERIFY(mTreePluginWidget->isSortingEnabled());
    QVERIFY(mTreePluginWidget->isHeaderHidden());
    QCOMPARE(mTreePluginWidget->selectionMode(), QAbstractItemView::SingleSelection);

    auto mSearchLineEdit = w.findChild<KTreeWidgetSearchLineWidget *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->searchLine()->text().isEmpty());
}

#include "moc_configurepluginswidgettest.cpp"
