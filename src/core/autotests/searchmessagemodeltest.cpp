/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagemodeltest.h"
#include "model/searchmessagemodel.h"
#include "test_model_helpers.h"
#include <QTest>

QTEST_GUILESS_MAIN(SearchMessageModelTest)

SearchMessageModelTest::SearchMessageModelTest(QObject *parent)
    : QObject(parent)
{
}

void SearchMessageModelTest::shouldHaveDefaultValue()
{
    SearchMessageModel w;
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(!w.loadSearchMessageInProgress());
    QVERIFY(w.stringNotFound());
}
