/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "directorycontainerwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "directory/directorycontainerwidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(DirectoryContainerWidgetTest)
DirectoryContainerWidgetTest::DirectoryContainerWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DirectoryContainerWidgetTest::shouldHaveDefaultValues()
{
    DirectoryContainerWidget w(nullptr, DirectoryWidget::DirectoryType::Room);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    // TODO
}

#include "moc_directorycontainerwidgettest.cpp"
