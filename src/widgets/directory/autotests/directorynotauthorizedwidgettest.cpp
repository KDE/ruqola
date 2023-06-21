/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorynotauthorizedwidgettest.h"
#include "directory/directorynotauthorizedwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(DirectoryNotAuthorizedWidgetTest)
DirectoryNotAuthorizedWidgetTest::DirectoryNotAuthorizedWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DirectoryNotAuthorizedWidgetTest::shouldHaveDefaultValues()
{
    DirectoryNotAuthorizedWidget w(nullptr);

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QCOMPARE(label->textFormat(), Qt::RichText);
}

#include "moc_directorynotauthorizedwidgettest.cpp"
