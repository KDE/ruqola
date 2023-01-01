/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "confirmpassworddialogtest.h"
#include "dialogs/confirmpassworddialog.h"
#include "dialogs/confirmpasswordwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfirmPasswordDialogTest)
ConfirmPasswordDialogTest::ConfirmPasswordDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ConfirmPasswordDialogTest::shouldHaveDefaultValues()
{
    ConfirmPasswordDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mConfirmPasswordWidget = w.findChild<ConfirmPasswordWidget *>(QStringLiteral("mConfirmPasswordWidget"));
    QVERIFY(mConfirmPasswordWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Ok | QDialogButtonBox::Cancel});
}
