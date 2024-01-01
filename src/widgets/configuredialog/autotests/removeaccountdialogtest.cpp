/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountdialogtest.h"
#include "configuredialog/removeaccountdialog.h"
#include "configuredialog/removeaccountwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(RemoveAccountDialogTest)
RemoveAccountDialogTest::RemoveAccountDialogTest(QObject *parent)
    : QObject{parent}
{
}

void RemoveAccountDialogTest::shouldHaveDefaultValues()
{
    RemoveAccountDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mRemoveAccountWidget = d.findChild<RemoveAccountWidget *>(QStringLiteral("mRemoveAccountWidget"));
    QVERIFY(mRemoveAccountWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QVERIFY(!d.removeLogs());
}

#include "moc_removeaccountdialogtest.cpp"
