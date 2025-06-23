/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeydialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mE2ePasswordDecodeKeyWidget = d.findChild<E2ePasswordDecodeKeyWidget *>(u"mE2ePasswordDecodeKeyWidget"_s);
    QVERIFY(mE2ePasswordDecodeKeyWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(d.password().isEmpty());
}

#include "moc_e2epassworddecodekeydialogtest.cpp"
