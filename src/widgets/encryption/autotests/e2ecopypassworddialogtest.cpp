/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypassworddialogtest.h"
#include "encryption/e2ecopypassworddialog.h"
#include "encryption/e2ecopypasswordwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(E2eCopyPasswordDialogTest)
E2eCopyPasswordDialogTest::E2eCopyPasswordDialogTest(QObject *parent)
    : QObject{parent}
{
}

void E2eCopyPasswordDialogTest::shouldHaveDefaultValues()
{
    E2eCopyPasswordDialog d(nullptr);

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto buttonBox = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Cancel | QDialogButtonBox::Ok});

    auto mE2eCopyPasswordWidget = d.findChild<E2eCopyPasswordWidget *>(QStringLiteral("mE2eCopyPasswordWidget"));
    QVERIFY(mE2eCopyPasswordWidget);
}

#include "moc_e2ecopypassworddialogtest.cpp"
