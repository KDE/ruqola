/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "common/flowlayout.h"
#include "dialogs/adduserscompletionlineedit.h"
#include "misc/adduserswidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AddUsersWidgetTest)
AddUsersWidgetTest::AddUsersWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AddUsersWidgetTest::shouldHaveDefaultValues()
{
    AddUsersWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchUserLineEdit = w.findChild<AddUsersCompletionLineEdit *>(u"mSearchUserLineEdit"_s);
    QVERIFY(mSearchUserLineEdit);
    QVERIFY(mSearchUserLineEdit->placeholderText().isEmpty());

    auto mFlowLayout = w.findChild<FlowLayout *>(u"mFlowLayout"_s);
    QVERIFY(mFlowLayout);

    QVERIFY(w.userIds().isEmpty());

    QVERIFY(w.placeHolderText().isEmpty());
}

void AddUsersWidgetTest::shouldChangePlaceHolderText()
{
    AddUsersWidget w(nullptr);
    const QString str = u"goo"_s;
    QVERIFY(w.placeHolderText().isEmpty());
    w.setPlaceholderText(str);
    QCOMPARE(w.placeHolderText(), str);

    auto mSearchUserLineEdit = w.findChild<AddUsersCompletionLineEdit *>(u"mSearchUserLineEdit"_s);
    QCOMPARE(mSearchUserLineEdit->placeholderText(), str);
}

#include "moc_adduserswidgettest.cpp"
