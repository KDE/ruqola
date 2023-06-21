/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompleterfilterproxymodeltest.h"
#include "model/usercompleterfilterproxymodel.h"
#include "model/usercompletermodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(UserCompleterFilterProxyModelTest)

UserCompleterFilterProxyModelTest::UserCompleterFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void UserCompleterFilterProxyModelTest::shouldHaveDefaultValue()
{
    UserCompleterFilterProxyModel w;
    QCOMPARE(w.rowCount(), 0);
    UserCompleterModel source;
    w.setSourceModel(&source);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(w.sourceModel(), &source);
}

#include "moc_usercompleterfilterproxymodeltest.cpp"
