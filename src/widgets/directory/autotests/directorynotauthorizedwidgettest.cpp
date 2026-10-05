/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorynotauthorizedwidgettest.h"

#include "directory/directorynotauthorizedwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(DirectoryNotAuthorizedWidgetTest)
DirectoryNotAuthorizedWidgetTest::DirectoryNotAuthorizedWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DirectoryNotAuthorizedWidgetTest::shouldHaveDefaultValues()
{
    const DirectoryNotAuthorizedWidget w(nullptr);

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QCOMPARE(label->textFormat(), Qt::RichText);
}

#include "moc_directorynotauthorizedwidgettest.cpp"
