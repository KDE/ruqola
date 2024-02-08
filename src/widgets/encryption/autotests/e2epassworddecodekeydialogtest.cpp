/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeydialogtest.h"
#include "encryption/e2epassworddecodekeydialog.h"
#include "encryption/e2epassworddecodekeywidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(E2ePasswordDecodeKeyDialogTest)
E2ePasswordDecodeKeyDialogTest::E2ePasswordDecodeKeyDialogTest(QObject *parent)
    : QObject{parent}
{
}

void E2ePasswordDecodeKeyDialogTest::shouldHaveDefaultValues()
{
    E2ePasswordDecodeKeyDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mE2ePasswordDecodeKeyWidget = d.findChild<E2ePasswordDecodeKeyWidget *>(QStringLiteral("mE2ePasswordDecodeKeyWidget"));
    QVERIFY(mE2ePasswordDecodeKeyWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_e2epassworddecodekeydialogtest.cpp"
