/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitieswidgettest.h"
#include "configurenewserver/configureactivitieswidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ConfigureActivitiesWidgetTest)
ConfigureActivitiesWidgetTest::ConfigureActivitiesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigureActivitiesWidgetTest::shouldHaveDefaultValues()
{
    ConfigureActivitiesWidget w;

    auto mListView = w.findChild<QListView *>("mListView"_L1);
    QVERIFY(mListView);
    QVERIFY(!mListView->isEnabled());
    QCOMPARE(mListView->selectionMode(), QAbstractItemView::MultiSelection);

    auto mainLayout = w.findChild<QVBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>("label"_L1);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QVERIFY(label->wordWrap());

    auto mEnableActivitiesSupport = w.findChild<QCheckBox *>("mEnableActivitiesSupport"_L1);
    QVERIFY(mEnableActivitiesSupport);
    QVERIFY(!mEnableActivitiesSupport->text().isEmpty());
    QVERIFY(!mEnableActivitiesSupport->isChecked());
}

#include "moc_configureactivitieswidgettest.cpp"
