/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddialogtest.h"
#include "encryption/e2epassworddialog.h"
#include "encryption/e2epasswordwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(E2ePasswordDialogTest)
E2ePasswordDialogTest::E2ePasswordDialogTest(QObject *parent)
    : QObject{parent}
{
}

void E2ePasswordDialogTest::shouldHaveDefaultValues()
{
    E2ePasswordDialog d(nullptr);

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto buttonBox = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Cancel | QDialogButtonBox::Ok});

    auto mE2ePasswordWidget = d.findChild<E2ePasswordWidget *>(QStringLiteral("mE2ePasswordWidget"));
    QVERIFY(mE2ePasswordWidget);
}

#include "moc_e2epassworddialogtest.cpp"
