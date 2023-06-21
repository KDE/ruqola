/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorywidgettest.h"
#include "directory/directorywidget.h"
#include "misc/searchwithdelaylineedit.h"
#include <QHeaderView>
#include <QLabel>
#include <QTest>
#include <QTreeView>
#include <QVBoxLayout>
QTEST_MAIN(DirectoryWidgetTest)
DirectoryWidgetTest::DirectoryWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void DirectoryWidgetTest::shouldHaveDefaultValues()
{
    DirectoryWidget w(nullptr, DirectoryWidget::Unknown);
    auto mLabelResultSearch = w.findChild<QLabel *>(QStringLiteral("mLabelResultSearch"));
    QVERIFY(mLabelResultSearch);
    QVERIFY(mLabelResultSearch->text().isEmpty());

    auto mSearchLineEdit = w.findChild<SearchWithDelayLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTreeView = w.findChild<QTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(mTreeView->isSortingEnabled());
    QCOMPARE(mTreeView->contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(mTreeView->header()->sectionsClickable());
}

#include "moc_directorywidgettest.cpp"
