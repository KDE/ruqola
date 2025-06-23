/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "confirmpassworddialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mConfirmPasswordWidget = w.findChild<ConfirmPasswordWidget *>(u"mConfirmPasswordWidget"_s);
    QVERIFY(mConfirmPasswordWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Ok | QDialogButtonBox::Cancel});
}

#include "moc_confirmpassworddialogtest.cpp"
