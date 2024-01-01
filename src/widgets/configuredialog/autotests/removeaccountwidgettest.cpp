/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "removeaccountwidgettest.h"
#include "configuredialog/removeaccountwidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(RemoveAccountWidgetTest)
RemoveAccountWidgetTest::RemoveAccountWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RemoveAccountWidgetTest::shouldHaveDefaultValues()
{
    RemoveAccountWidget w;
    auto mRemoveLogs = w.findChild<QCheckBox *>(QStringLiteral("mRemoveLogs"));
    QVERIFY(mRemoveLogs);
    QVERIFY(!mRemoveLogs->text().isEmpty());
    QVERIFY(!mRemoveLogs->isChecked());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    QVERIFY(!w.removeLogs());

    auto mInfo = w.findChild<QLabel *>(QStringLiteral("mInfo"));
    QVERIFY(mInfo);
    QVERIFY(mInfo->text().isEmpty());
}

#include "moc_removeaccountwidgettest.cpp"
