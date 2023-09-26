/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagefilterproxymodeltest.h"
#include "model/commonmessagemodel.h"
#include "model/searchmessagefilterproxymodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(SearchMessageFilterProxyModelTest)

SearchMessageFilterProxyModelTest::SearchMessageFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void SearchMessageFilterProxyModelTest::shouldHaveDefaultValue()
{
    SearchMessageFilterProxyModel w;
    CommonMessageModel sourceModel;
    w.setSourceModel(&sourceModel);
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.sourceModel());
    QCOMPARE(w.sourceModel(), &sourceModel);
}

#include "moc_searchmessagefilterproxymodeltest.cpp"
