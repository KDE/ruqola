/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdirectmessageswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/createdirectmessageswidget.h"
#include "misc/adduserswidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateDirectMessagesWidgetTest)
CreateDirectMessagesWidgetTest::CreateDirectMessagesWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateDirectMessagesWidgetTest::shouldHaveDefaultValues()
{
    CreateDirectMessagesWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mUsers = w.findChild<AddUsersWidget *>(u"mUsers"_s);
    QVERIFY(mUsers);
    QVERIFY(!mUsers->placeHolderText().isEmpty());

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QVERIFY(label->wordWrap());

    QVERIFY(w.userNames().isEmpty());

    auto mTooManyUsers = w.findChild<QLabel *>(u"mTooManyUsers"_s);
    QVERIFY(mTooManyUsers);
    QVERIFY(!mTooManyUsers->text().isEmpty());
    QVERIFY(!mTooManyUsers->isVisible());
}

#include "moc_createdirectmessageswidgettest.cpp"
