/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagefilterproxymodeltest.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/commonmessagesmodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(CommonMessageFilterProxyModelTest)

CommonMessageFilterProxyModelTest::CommonMessageFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void CommonMessageFilterProxyModelTest::shouldHaveDefaultValue()
{
    CommonMessageFilterProxyModel w;
    CommonMessagesModel sourceModel;
    w.setSourceModel(&sourceModel);
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.sourceModel());
    QCOMPARE(w.sourceModel(), &sourceModel);
}

#include "moc_commonmessagefilterproxymodeltest.cpp"
