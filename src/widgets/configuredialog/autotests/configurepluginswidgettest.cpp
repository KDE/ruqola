/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QCOMPARE(mainLayout->spacing(), 0);

    auto mTreePluginWidget = w.findChild<QTreeWidget *>(u"mTreePluginWidget"_s);
    QVERIFY(mTreePluginWidget);
    QVERIFY(mTreePluginWidget->isSortingEnabled());
    QVERIFY(mTreePluginWidget->isHeaderHidden());
    QCOMPARE(mTreePluginWidget->selectionMode(), QAbstractItemView::SingleSelection);

    auto mSearchLineEdit = w.findChild<KTreeWidgetSearchLineWidget *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->searchLine()->text().isEmpty());
}

#include "moc_configurepluginswidgettest.cpp"
