/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statusmodeltest.h"
#include "model/statusmodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(StatusModelTest)

StatusModelTest::StatusModelTest(QObject *parent)
    : QObject(parent)
{
}

void StatusModelTest::shouldHaveDefaultValue()
{
    StatusModel w;
    QCOMPARE(w.rowCount(), 5);

    QCOMPARE(w.currentStatus(), 0);
    QVERIFY(w.customText().isEmpty());
}

// TODO add autotest about currentStatus changed !

#include "moc_statusmodeltest.cpp"
