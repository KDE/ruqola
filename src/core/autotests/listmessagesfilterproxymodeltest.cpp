/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listmessagesfilterproxymodeltest.h"
#include "model/listmessagesfilterproxymodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(ListMessagesFilterProxyModelTest)

ListMessagesFilterProxyModelTest::ListMessagesFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void ListMessagesFilterProxyModelTest::shouldHaveDefaultValues()
{
    ListMessagesModel model;
    ListMessagesFilterProxyModel w(&model);
    QCOMPARE(w.filterCaseSensitivity(), Qt::CaseInsensitive);
    QCOMPARE(w.sortOrder(), Qt::DescendingOrder);
    QCOMPARE(w.filterRole(), ListMessagesModel::OriginalMessage);
}

#include "moc_listmessagesfilterproxymodeltest.cpp"
