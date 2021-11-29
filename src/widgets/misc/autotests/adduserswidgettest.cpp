/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserswidgettest.h"
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
    AddUsersWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mSearchUserLineEdit = w.findChild<AddUsersCompletionLineEdit *>(QStringLiteral("mSearchUserLineEdit"));
    QVERIFY(mSearchUserLineEdit);
    QVERIFY(mSearchUserLineEdit->placeholderText().isEmpty());

    auto mFlowLayout = w.findChild<FlowLayout *>(QStringLiteral("mFlowLayout"));
    QVERIFY(mFlowLayout);

    QVERIFY(w.userIds().isEmpty());
}
