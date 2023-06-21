/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listmessagesmodelfilterproxymodeltest.h"
#include "model/listmessagesmodelfilterproxymodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(ListMessagesModelFilterProxyModelTest)

ListMessagesModelFilterProxyModelTest::ListMessagesModelFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void ListMessagesModelFilterProxyModelTest::shouldHaveDefaultValues()
{
    ListMessagesModel model;
    ListMessagesModelFilterProxyModel w(&model);
    QCOMPARE(w.filterCaseSensitivity(), Qt::CaseInsensitive);
    QCOMPARE(w.sortOrder(), Qt::DescendingOrder);
    QCOMPARE(w.filterRole(), ListMessagesModel::OriginalMessage);
}

#include "moc_listmessagesmodelfilterproxymodeltest.cpp"
