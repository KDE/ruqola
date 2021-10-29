/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mTreeView = w.findChild<QTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(mTreeView->isSortingEnabled());
    QCOMPARE(mTreeView->contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(mTreeView->header()->sectionsClickable());
}
