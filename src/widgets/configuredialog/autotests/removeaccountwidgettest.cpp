/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "removeaccountwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mRemoveLogs = w.findChild<QCheckBox *>(u"mRemoveLogs"_s);
    QVERIFY(mRemoveLogs);
    QVERIFY(!mRemoveLogs->text().isEmpty());
    QVERIFY(!mRemoveLogs->isChecked());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    QVERIFY(!w.removeLogs());

    auto mInfo = w.findChild<QLabel *>(u"mInfo"_s);
    QVERIFY(mInfo);
    QVERIFY(mInfo->text().isEmpty());
}

#include "moc_removeaccountwidgettest.cpp"
