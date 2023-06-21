/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdirectmessageswidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mUsers = w.findChild<AddUsersWidget *>(QStringLiteral("mUsers"));
    QVERIFY(mUsers);
    QVERIFY(!mUsers->placeHolderText().isEmpty());

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QVERIFY(label->wordWrap());

    QVERIFY(w.userNames().isEmpty());

    auto mTooManyUsers = w.findChild<QLabel *>(QStringLiteral("mTooManyUsers"));
    QVERIFY(mTooManyUsers);
    QVERIFY(!mTooManyUsers->text().isEmpty());
    QVERIFY(!mTooManyUsers->isVisible());
}

#include "moc_createdirectmessageswidgettest.cpp"
