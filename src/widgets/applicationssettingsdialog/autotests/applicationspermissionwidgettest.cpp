/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationspermissionwidgettest.h"
#include "applicationssettingsdialog/applicationspermissionwidget.h"

#include <QTest>
#include <QTextEdit>
#include <QVBoxLayout>
QTEST_MAIN(ApplicationsPermissionWidgetTest)
ApplicationsPermissionWidgetTest::ApplicationsPermissionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsPermissionWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsPermissionWidget d;
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTextEdit = d.findChild<QTextEdit *>(QStringLiteral("mTextEdit"));
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->toPlainText().isEmpty());
}

#include "moc_applicationspermissionwidgettest.cpp"
