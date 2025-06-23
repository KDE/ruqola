/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mRemoveAccountWidget = d.findChild<RemoveAccountWidget *>(u"mRemoveAccountWidget"_s);
    QVERIFY(mRemoveAccountWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(!d.removeLogs());
}

#include "moc_removeaccountdialogtest.cpp"
